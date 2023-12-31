#include "image_provider_net.h"




#include "ImageProcUtils.h"

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"


#include<iostream>
#include<fstream>
#include<signal.h>
#include <string>
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>
#include <cstdlib>
#include <assert.h>

using namespace std;
using namespace cv;

using image_proc_utils::ImgData;
using image_proc_utils::ImageProvider;




RTSPClient* image_proc_utils::ImageProviderNet::rtspClient = NULL;
char image_proc_utils::ImageProviderNet::eventLoopWatchVariable = 0;


// A function that outputs a string that identifies each stream (for debugging output).  Modify this if you wish:
UsageEnvironment& operator<<(UsageEnvironment& env, const RTSPClient& rtspClient) {
  return env << "[URL:\"" << rtspClient.url() << "\"]: ";
}

// A function that outputs a string that identifies each subsession (for debugging output).  Modify this if you wish:
UsageEnvironment& operator<<(UsageEnvironment& env, const MediaSubsession& subsession) {
  return env << subsession.mediumName() << "/" << subsession.codecName();
}


image_proc_utils::ImageProviderNet::ImageProviderNet(const std::string& _url) : url(_url), ImageProvider() {

}

image_proc_utils::ImageProviderNet::~ImageProviderNet() {

}


void* image_proc_utils::ImageProviderNet::thread_function(void* arg) {

    image_proc_utils::ImageProviderNet* provider = (image_proc_utils::ImageProviderNet*) arg;
    try {
		TaskScheduler* scheduler = BasicTaskScheduler::createNew();
		UsageEnvironment* env = BasicUsageEnvironment::createNew(*scheduler);

        openURL(*env, "client", provider->url.c_str(), provider);
		// All subsequent activity takes place within the event loop:
		env->taskScheduler().doEventLoop(&eventLoopWatchVariable);
    	// This function call does not return, unless, at some point in time, "eventLoopWatchVariable" gets set to something non-zero.
    }
    catch(const exception& e) {
        cout << "Error1: " << string(e.what()) << endl;
    }

    provider->initialized = false;
    cout << "doneeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee !!!" << endl;

    return NULL;

}

void image_proc_utils::ImageProviderNet::Initialize(Callback callback) {

    if (initialized == true) {
        THROW_EX(RobotException, "already initialized");
    }

    ImageProvider::Initialize(callback);    

    pthread_create(&t_id,NULL,thread_function, (void*) this);


}

void image_proc_utils::ImageProviderNet::Finalize() {
    
    
    if (initialized == false) {
        return;
    }


    
    

    shutdownStream(rtspClient);    

    pthread_join(this->t_id, NULL);
    sleep(1);

    ImageProvider::Finalize();
    
}





#define RTSP_CLIENT_VERBOSITY_LEVEL 10 // by default, print verbose output from each "RTSPClient"

static unsigned rtspClientCount = 0; // Counts how many streams (i.e., "RTSPClient"s) are currently in use.

void image_proc_utils::ImageProviderNet::openURL(UsageEnvironment& env, char const* progName, char const* rtspURL, const ImageProvider* provider) {
    // Begin by creating a "RTSPClient" object.  Note that there is a separate "RTSPClient" object for each stream that we wish
    // to receive (even if more than stream uses the same "rtsp://" URL).
    rtspClient = ourRTSPClient::createNew(env, rtspURL, provider, RTSP_CLIENT_VERBOSITY_LEVEL, progName);
        if (rtspClient == NULL) {
        env << "Failed to create a RTSP client for URL \"" << rtspURL << "\": " << env.getResultMsg() << "\n";
        return;
    }

    ++rtspClientCount;

    // Next, send a RTSP "DESCRIBE" command, to get a SDP description for the stream.
    // Note that this command - like all RTSP commands - is sent asynchronously; we do not block, waiting for a response.
    // Instead, the following function call returns immediately, and we handle the RTSP response later, from within the event loop:
    rtspClient->sendDescribeCommand(continueAfterDESCRIBE); 
}


// Implementation of the RTSP 'response handlers':

void image_proc_utils::ImageProviderNet::continueAfterDESCRIBE(RTSPClient* rtspClient, int resultCode, char* resultString) {
    do {
        UsageEnvironment& env = rtspClient->envir(); // alias
        StreamClientState& scs = ((ourRTSPClient*)rtspClient)->scs; // alias

        if (resultCode != 0) {
            env << *rtspClient << "Failed to get a SDP description: " << resultString << "\n";
            delete[] resultString;
            break;
        }

        char* const sdpDescription = resultString;
        env << *rtspClient << "Got a SDP description:\n" << sdpDescription << "\n";

        // Create a media session object from this SDP description:
        scs.session = MediaSession::createNew(env, sdpDescription);
        delete[] sdpDescription; // because we don't need it anymore
        if (scs.session == NULL) {
            env << *rtspClient << "Failed to create a MediaSession object from the SDP description: " << env.getResultMsg() << "\n";
            break;
        } else if (!scs.session->hasSubsessions()) {
            env << *rtspClient << "This session has no media subsessions (i.e., no \"m=\" lines)\n";
            break;
        }

        // Then, create and set up our data source objects for the session.  We do this by iterating over the session's 'subsessions',
        // calling "MediaSubsession::initiate()", and then sending a RTSP "SETUP" command, on each one.
        // (Each 'subsession' will have its own data source.)
        scs.iter = new MediaSubsessionIterator(*scs.session);
        setupNextSubsession(rtspClient);
        return;
    } while (0);

    // An unrecoverable error occurred with this stream.
    shutdownStream(rtspClient);
}

// By default, we request that the server stream its data using RTP/UDP.
// If, instead, you want to request that the server stream via RTP-over-TCP, change the following to True:
#define REQUEST_STREAMING_OVER_TCP False

void image_proc_utils::ImageProviderNet::setupNextSubsession(RTSPClient* rtspClient) {
    UsageEnvironment& env = rtspClient->envir(); // alias
    StreamClientState& scs = ((ourRTSPClient*)rtspClient)->scs; // alias

    scs.subsession = scs.iter->next();
    if (scs.subsession != NULL) {
        if (!scs.subsession->initiate()) {
            env << *rtspClient << "Failed to initiate the \"" << *scs.subsession << "\" subsession: " << env.getResultMsg() << "\n";
            setupNextSubsession(rtspClient); // give up on this subsession; go to the next one
        } else {
            env << *rtspClient << "Initiated the \"" << *scs.subsession << "\" subsession (";
            if (scs.subsession->rtcpIsMuxed()) {
        env << "client port " << scs.subsession->clientPortNum();
            } else {
        env << "client ports " << scs.subsession->clientPortNum() << "-" << scs.subsession->clientPortNum()+1;
            }
            env << ")\n";

            // Continue setting up this subsession, by sending a RTSP "SETUP" command:
            rtspClient->sendSetupCommand(*scs.subsession, continueAfterSETUP, False, REQUEST_STREAMING_OVER_TCP);
        }
        return;
    }

    // We've finished setting up all of the subsessions.  Now, send a RTSP "PLAY" command to start the streaming:
    if (scs.session->absStartTime() != NULL) {
        // Special case: The stream is indexed by 'absolute' time, so send an appropriate "PLAY" command:
        rtspClient->sendPlayCommand(*scs.session, continueAfterPLAY, scs.session->absStartTime(), scs.session->absEndTime());
    } else {
        scs.duration = scs.session->playEndTime() - scs.session->playStartTime();
        rtspClient->sendPlayCommand(*scs.session, continueAfterPLAY);
    }
}

void image_proc_utils::ImageProviderNet::continueAfterSETUP(RTSPClient* rtspClient, int resultCode, char* resultString) {
    do {
        UsageEnvironment& env = rtspClient->envir(); // alias
        StreamClientState& scs = ((ourRTSPClient*)rtspClient)->scs; // alias
        const ImageProvider* provider = ((ourRTSPClient*)rtspClient)->provider;

        if (resultCode != 0) {
            env << *rtspClient << "Failed to set up the \"" << *scs.subsession << "\" subsession: " << resultString << "\n";
            break;
        }

        env << *rtspClient << "Set up the \"" << *scs.subsession << "\" subsession (";
        if (scs.subsession->rtcpIsMuxed()) {
            env << "client port " << scs.subsession->clientPortNum();
        } else {
            env << "client ports " << scs.subsession->clientPortNum() << "-" << scs.subsession->clientPortNum()+1;
        }
        env << ")\n";

        // Having successfully setup the subsession, create a data sink for it, and call "startPlaying()" on it.
        // (This will prepare the data sink to receive data; the actual flow of data from the client won't start happening until later,
        // after we've sent a RTSP "PLAY" command.)

        scs.subsession->sink = DummySink::createNew(env, *scs.subsession, provider, rtspClient->url());
            // perhaps use your own custom "MediaSink" subclass instead
        if (scs.subsession->sink == NULL) {
            env << *rtspClient << "Failed to create a data sink for the \"" << *scs.subsession
            << "\" subsession: " << env.getResultMsg() << "\n";
            break;
        }

        env << *rtspClient << "Created a data sink for the \"" << *scs.subsession << "\" subsession\n";
        scs.subsession->miscPtr = rtspClient; // a hack to let subsession handler functions get the "RTSPClient" from the subsession 
        scs.subsession->sink->startPlaying(*(scs.subsession->readSource()),
                            subsessionAfterPlaying, scs.subsession);
        // Also set a handler to be called if a RTCP "BYE" arrives for this subsession:
        if (scs.subsession->rtcpInstance() != NULL) {
            scs.subsession->rtcpInstance()->setByeWithReasonHandler(subsessionByeHandler, scs.subsession);
        }
    } while (0);
    delete[] resultString;

    // Set up the next subsession, if any:
    setupNextSubsession(rtspClient);
}

void image_proc_utils::ImageProviderNet::continueAfterPLAY(RTSPClient* rtspClient, int resultCode, char* resultString) {
    Boolean success = False;

    do {
        UsageEnvironment& env = rtspClient->envir(); // alias
        StreamClientState& scs = ((ourRTSPClient*)rtspClient)->scs; // alias

        if (resultCode != 0) {
            env << *rtspClient << "Failed to start playing session: " << resultString << "\n";
            break;
        }

        // Set a timer to be handled at the end of the stream's expected duration (if the stream does not already signal its end
        // using a RTCP "BYE").  This is optional.  If, instead, you want to keep the stream active - e.g., so you can later
        // 'seek' back within it and do another RTSP "PLAY" - then you can omit this code.
        // (Alternatively, if you don't want to receive the entire stream, you could set this timer for some shorter value.)
        if (scs.duration > 0) {
            unsigned const delaySlop = 2; // number of seconds extra to delay, after the stream's expected duration.  (This is optional.)
            scs.duration += delaySlop;
            unsigned uSecsToDelay = (unsigned)(scs.duration*1000000);
            scs.streamTimerTask = env.taskScheduler().scheduleDelayedTask(uSecsToDelay, (TaskFunc*)streamTimerHandler, rtspClient);
        }

        env << *rtspClient << "Started playing session";
        if (scs.duration > 0) {
            env << " (for up to " << scs.duration << " seconds)";
        }
        env << "...\n";

        success = True;
        } while (0);

        delete[] resultString;

        if (!success) {
            // An unrecoverable error occurred with this stream.
            shutdownStream(rtspClient);
        }
}


// Implementation of the other event handlers:

void image_proc_utils::ImageProviderNet::subsessionAfterPlaying(void* clientData) {
    MediaSubsession* subsession = (MediaSubsession*)clientData;
    RTSPClient* rtspClient = (RTSPClient*)(subsession->miscPtr);

    // Begin by closing this subsession's stream:
    Medium::close(subsession->sink);
    subsession->sink = NULL;

    // Next, check whether *all* subsessions' streams have now been closed:
    MediaSession& session = subsession->parentSession();
    MediaSubsessionIterator iter(session);
    while ((subsession = iter.next()) != NULL) {
        if (subsession->sink != NULL) return; // this subsession is still active
    }

    // All subsessions' streams have now been closed, so shutdown the client:
    shutdownStream(rtspClient);
}

void image_proc_utils::ImageProviderNet::subsessionByeHandler(void* clientData, char const* reason) {
    MediaSubsession* subsession = (MediaSubsession*)clientData;
    RTSPClient* rtspClient = (RTSPClient*)subsession->miscPtr;
    UsageEnvironment& env = rtspClient->envir(); // alias

    env << *rtspClient << "Received RTCP \"BYE\"";
    if (reason != NULL) {
        env << " (reason:\"" << reason << "\")";
        delete[] reason;
    }
    env << " on \"" << *subsession << "\" subsession\n";

    // Now act as if the subsession had closed:
    subsessionAfterPlaying(subsession);
}

void image_proc_utils::ImageProviderNet::streamTimerHandler(void* clientData) {
    ourRTSPClient* rtspClient = (ourRTSPClient*)clientData;
    StreamClientState& scs = rtspClient->scs; // alias

    scs.streamTimerTask = NULL;

    // Shut down the stream:
    shutdownStream(rtspClient);
}

void image_proc_utils::ImageProviderNet::shutdownStream(RTSPClient* rtspClient, int exitCode) {
    UsageEnvironment& env = rtspClient->envir(); // alias
    StreamClientState& scs = ((ourRTSPClient*)rtspClient)->scs; // alias

    // First, check whether any subsessions have still to be closed:
    if (scs.session != NULL) { 
        env << "session not null" << "\n";
        Boolean someSubsessionsWereActive = False;
        MediaSubsessionIterator iter(*scs.session);
        MediaSubsession* subsession;

        while ((subsession = iter.next()) != NULL) {
            if (subsession->sink != NULL) {
                Medium::close(subsession->sink);
                subsession->sink = NULL;
                env << "sink = NULL\n";
                if (subsession->rtcpInstance() != NULL) {
                    env << "step 1";
                    subsession->rtcpInstance()->setByeHandler(NULL, NULL); // in case the server sends a RTCP "BYE" while handling "TEARDOWN"
                    env << "step 2";
                }

                env << "step 3";
                someSubsessionsWereActive = True;
            }
            env << "next\n"; 
        }

        env << "next 1 " << someSubsessionsWereActive << "\n";
        if (someSubsessionsWereActive) {
            // Send a RTSP "TEARDOWN" command, to tell the server to shutdown the stream.
            // Don't bother handling the response to the "TEARDOWN".
            env << "next 2 " << scs.session << "\n";
            rtspClient->sendTeardownCommand(*scs.session, NULL);
        }

        env << "next 3 \n";
    }

     
    env << *rtspClient << "Closing the stream..." << rtspClientCount << "\n";

    Medium::close(rtspClient);
    // Note that this will also cause this stream's "StreamClientState" structure to get reclaimed.


    if (--rtspClientCount == 0) {
        env << "done\n";
    // The final stream has ended, so exit the application now.
    // (Of course, if you're embedding this code into your own application, you might want to comment this out,

    // and replace it with "eventLoopWatchVariable = 1;", so that we leave the LIVE555 event loop, and continue running "main()".)

        eventLoopWatchVariable = 1;
        //exit(exitCode);
    }

 
}




image_proc_utils::ourRTSPClient* image_proc_utils::ourRTSPClient::createNew(UsageEnvironment& env, char const* rtspURL, const ImageProvider* _provider,
					int verbosityLevel, char const* applicationName, portNumBits tunnelOverHTTPPortNum) {
    return new ourRTSPClient(env, rtspURL, _provider, verbosityLevel, applicationName, tunnelOverHTTPPortNum);
}

image_proc_utils::ourRTSPClient::ourRTSPClient(UsageEnvironment& env, char const* rtspURL, const ImageProvider* _provider,
			     int verbosityLevel, char const* applicationName, portNumBits tunnelOverHTTPPortNum) 
    : RTSPClient(env,rtspURL, verbosityLevel, applicationName, tunnelOverHTTPPortNum, -1), provider(_provider) {
}

image_proc_utils::ourRTSPClient::~ourRTSPClient() {
}


// Implementation of "StreamClientState":

image_proc_utils::StreamClientState::StreamClientState()
    : iter(NULL), session(NULL), subsession(NULL), streamTimerTask(NULL), duration(0.0) {
}

image_proc_utils::StreamClientState::~StreamClientState() {
    delete iter;
    if (session != NULL) {
        // We also need to delete "session", and unschedule "streamTimerTask" (if set)
        UsageEnvironment& env = session->envir(); // alias

        env.taskScheduler().unscheduleDelayedTask(streamTimerTask);
        Medium::close(session);
    }
}


// Implementation of "DummySink":

// Even though we're not going to be doing anything with the incoming data, we still need to receive it.
// Define the size of the buffer that we'll use:
#define DUMMY_SINK_RECEIVE_BUFFER_SIZE 1000000


image_proc_utils::DummySink* image_proc_utils::DummySink::createNew(UsageEnvironment& env, MediaSubsession& subsession, const ImageProvider* _provider, char const* streamId) {
    return new DummySink(env, subsession, _provider, streamId);
}

image_proc_utils::DummySink::DummySink(UsageEnvironment& env, MediaSubsession& subsession, const ImageProvider* _provider, char const* streamId)
    : MediaSink(env)
    ,fSubsession(subsession)
    , index(0) 
    , provider(_provider)
{
    fStreamId = strDup(streamId);
    fReceiveBuffer = new u_int8_t[DUMMY_SINK_RECEIVE_BUFFER_SIZE];
}

image_proc_utils::DummySink::~DummySink() {
    delete[] fReceiveBuffer;
    delete[] fStreamId;
}

void image_proc_utils::DummySink::afterGettingFrame(void* clientData, unsigned frameSize, unsigned numTruncatedBytes,
				  struct timeval presentationTime, unsigned durationInMicroseconds) {
    DummySink* sink = (DummySink*)clientData;
    
    sink->afterGettingFrame(frameSize, numTruncatedBytes, presentationTime, durationInMicroseconds);
}

// If you don't want to see debugging output for each received frame, then comment out the following line:
//#define DEBUG_PRINT_EACH_RECEIVED_FRAME 1

void image_proc_utils::DummySink::afterGettingFrame(unsigned frameSize, unsigned numTruncatedBytes,
				  struct timeval presentationTime, unsigned /*durationInMicroseconds*/) {
  // We've just received a frame of data.  (Optionally) print out information about it:
    
	char str[256];
	//image_proc_utils::print_time(str);
	//printf("seq %06d frame received at: %s\n", index, str);

#ifdef DEBUG_PRINT_EACH_RECEIVED_FRAME 
    if (fStreamId != NULL) 
        envir() << "Stream \"" << fStreamId << "\"; ";
      
    envir() << fSubsession.mediumName() << "/" << fSubsession.codecName() << ":\tReceived " << frameSize << " bytes";
    if (numTruncatedBytes > 0) 
        envir() << " (with " << numTruncatedBytes << " bytes truncated)";

    char uSecsStr[6+1]; // used to output the 'microseconds' part of the presentation time
    sprintf(uSecsStr, "%06u", (unsigned)presentationTime.tv_usec);
    envir() << ".\tPresentation time: " << (int)presentationTime.tv_sec << "." << uSecsStr;
    if (fSubsession.rtpSource() != NULL && !fSubsession.rtpSource()->hasBeenSynchronizedUsingRTCP()) {
        envir() << "!"; // mark the debugging output to indicate that this presentation time is not RTCP-synchronized
    }
#ifdef DEBUG_PRINT_NPT
  envir() << "\tNPT: " << fSubsession.getNormalPlayTime(presentationTime);
#endif
  envir() << "\n";
#endif

    
    int headerSize = sizeof(image_proc_utils::ImgData::image_header);
    int val = fReceiveBuffer[frameSize-(headerSize + 2)+0] & 0xff;
    int dataLen = frameSize-(headerSize + 2);

    const char * data = (char *)fReceiveBuffer;
    image_proc_utils::ImgData::image_header * p = (image_proc_utils::ImgData::image_header *)&data[dataLen];
    //
    //p->seq = index;
    p->imageSize = dataLen;

    //printf("%100s %s\n", "just before sending to detector:", print_time());
    //net->Detect(data, dataLen, (char *)&data[dataLen], headerSize);
    //data->Set
    //envir() << "data len " << dataLen << "\n";
    //commonData.SetData(data, dataLen, *p);
    // printf("val is %d headersize is %d\n", p->seq, headerSize); //(sizeof(header) + 2)]);
    if (index < 5 & 0) {

        char filename[256];
        sprintf(filename, "./temp/file%06d.jpg", val);
        //cout << filename << endl;
        ofstream wf(filename, ios::out | ios::binary);
        if (!wf) {
            cout << "Cannot open file: " << filename << endl;

        }
        else {
            wf.write((char *) fReceiveBuffer, frameSize-((headerSize + 2)));

        }
        wf.close();


    }

    index ++;
    if(provider) {
        const char * pHeader = (const char*) p;
        vector<char> image(data, data+dataLen);
        vector<char> header(pHeader, pHeader+sizeof(image_proc_utils::ImgData::image_header));

        //provider->callback(x, y);
       const image_proc_utils::ImageProviderNet * ptr = (const image_proc_utils::ImageProviderNet*) provider;
       ptr->callback(image, header);
       //cout << ptr->url << endl;
    }
    else 
        cout << "provier is null" << endl;
    //printf("%100s %s\n", "after detection:", print_time());
    // Then continue, to request the next frame of data:
    continuePlaying();
}

Boolean image_proc_utils::DummySink::continuePlaying() {
    if (fSource == NULL) return False; // sanity check (should not happen)

    // Request the next frame of data from our input source.  "afterGettingFrame()" will get called later, when it arrives:
    fSource->getNextFrame(fReceiveBuffer, DUMMY_SINK_RECEIVE_BUFFER_SIZE,
                        afterGettingFrame, this,
                        onSourceClosure, this);
    return True;
}


