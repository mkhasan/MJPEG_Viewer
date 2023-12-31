#ifndef _IMAGE_PROVIDER_NET_H
#define _IMAGE_PROVIDER_NET_H


#include "image_provider.h"

#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"

#include <string>

class DummySink;
//class image_proc_utils::StreamClientState;

namespace image_proc_utils {

    // If you're streaming just a single stream (i.e., just from a single URL, once), then you can define and use just a single
    // "StreamClientState" structure, as a global variable in your application.  However, because - in this demo application - we're
    // showing how to play multiple streams, concurrently, we can't do that.  Instead, we have to have a separate "StreamClientState"
    // structure for each "RTSPClient".  To do this, we subclass "RTSPClient", and add a "StreamClientState" field to the subclass:

 
  
    class StreamClientState {
    public:
        StreamClientState();
        virtual ~StreamClientState();

    public:
        MediaSubsessionIterator* iter;
        MediaSession* session;
        MediaSubsession* subsession;
        TaskToken streamTimerTask;
        double duration;
    }; 
    
    class ourRTSPClient: public RTSPClient {
    public:
        static ourRTSPClient* createNew(UsageEnvironment& env, char const* rtspURL, const ImageProvider* provider,
                        int verbosityLevel = 0,
                        char const* applicationName = NULL,
                        portNumBits tunnelOverHTTPPortNum = 0);

    protected:
        ourRTSPClient(UsageEnvironment& env, char const* rtspURL, const ImageProvider* provider, 
            int verbosityLevel, char const* applicationName, portNumBits tunnelOverHTTPPortNum);
        // called only by createNew();
        virtual ~ourRTSPClient();

    public:
        StreamClientState scs;
        const ImageProvider* provider;
    };

    // Define a data sink (a subclass of "MediaSink") to receive the data for each subsession (i.e., each audio or video 'substream').
    // In practice, this might be a class (or a chain of classes) that decodes and then renders the incoming audio or video.
    // Or it might be a "FileSink", for outputting the received data into a file (as is done by the "openRTSP" application).
    // In this example code, however, we define a simple 'dummy' sink that receives incoming data, but does nothing with it.

    class DummySink: public MediaSink {
    public:
        static DummySink* createNew(UsageEnvironment& env,
                        MediaSubsession& subsession, // identifies the kind of data that's being received
                        const ImageProvider* provider,
                        char const* streamId = NULL); // identifies the stream itself (optional)

    private:
        DummySink(UsageEnvironment& env, MediaSubsession& subsession, const ImageProvider* provider, char const* streamId);
        // called only by "createNew()"
        virtual ~DummySink();

        static void afterGettingFrame(void* clientData, unsigned frameSize,
                                    unsigned numTruncatedBytes,
                    struct timeval presentationTime,
                                    unsigned durationInMicroseconds);
        void afterGettingFrame(unsigned frameSize, unsigned numTruncatedBytes,
                    struct timeval presentationTime, unsigned durationInMicroseconds);

    private:
        // redefined virtual functions:
        virtual Boolean continuePlaying();

    private:
        u_int8_t* fReceiveBuffer;
        MediaSubsession& fSubsession;
        char* fStreamId;
        int index;
        const ImageProvider *provider;
    };


	class ImageProviderNet : public ImageProvider {

        friend DummySink;
		const int DURATION_MS = 10;
		//bool quit;

        std::string url;

    private:


        static RTSPClient* rtspClient;
        static char eventLoopWatchVariable;

		// The main streaming routine (for each "rtsp://" URL):
        static void openURL(UsageEnvironment& env, char const* progName, char const* rtspURL, const ImageProvider* provider = NULL);
        
        // RTSP 'response handlers':
        static void continueAfterDESCRIBE(RTSPClient* rtspClient, int resultCode, char* resultString);
        static void continueAfterSETUP(RTSPClient* rtspClient, int resultCode, char* resultString);
        static void continueAfterPLAY(RTSPClient* rtspClient, int resultCode, char* resultString);

        // Used to iterate through each stream's 'subsessions', setting up each one:
        static void setupNextSubsession(RTSPClient* rtspClient);
        
        // Other event handler functions:
        static void subsessionAfterPlaying(void* clientData); // called when a stream's subsession (e.g., audio or video substream) ends
        
        // called when a RTCP "BYE" is received for a subsession
        static void subsessionByeHandler(void* clientData, char const* reason);
        
        // called at the end of a stream's expected duration (if the stream has not already signaled its end using a RTCP "BYE")
        static void streamTimerHandler(void* clientData);
        
        
        // Used to shut down and close a stream (including its "RTSPClient" object):
        static void shutdownStream(RTSPClient* rtspClient, int exitCode = 1);

 
 


	public:
		

		ImageProviderNet(const std::string& _url);

		virtual ~ImageProviderNet();

		virtual void Initialize(Callback callback) override;
		virtual void Finalize() override;

        
    private:
        pthread_t t_id;
        static void* thread_function(void* arg);        

		

	};

}


#endif /* _IMAGE_PROVIDER_H */