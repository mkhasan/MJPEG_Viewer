#include "image_provider_shm.h"

#include "ImageProcUtils.h"

#include <iostream>
#include <exception>

using namespace std;

#ifdef _WIN32
image_proc_utils::ImageProviderShm::ImageProviderShm(ImageSource* _source)
	: ImageProvider(), source(_source), quit(false), hThread(NULL) {

}

#else
image_proc_utils::ImageProviderShm::ImageProviderShm(ImageSource* _source)
	: ImageProvider(), source(_source), quit(false), t_id(NULL) {

}
#endif

image_proc_utils::ImageProviderShm::~ImageProviderShm() {
	Finalize();
}

void image_proc_utils::ImageProviderShm::Initialize(Callback _callback) {

	if (initialized == true) {
		THROW_EX(RobotException, "already initialized");
	}

	callback = _callback;
	quit = false;

#ifdef _WIN32
	hThread = CreateThread(
		NULL,                   // default security attributes
		0,                      // use default stack size  
		ThreadFunction,       // thread function name
		this,          // argument to thread function 
		0,                      // use default creation flags 
		NULL);
	
	if (hThread == NULL) {
		THROW_EX(RobotException, "Error in creating thread");
	}

#else
    pthread_create(&t_id,NULL,thread_function, (void*) this);
#endif

	initialized = true;
}

void image_proc_utils::ImageProviderShm::Finalize() {

	if (initialized == false)
		return;
	quit = true;

#ifdef _WIN32
	WaitForSingleObject(hThread, INFINITE);
#else
    pthread_join(this->t_id, NULL);
#endif

	initialized = false;

}

#ifdef _WIN32
DWORD WINAPI image_proc_utils::ImageProviderShm::ThreadFunction(LPVOID lpParam) {
#else
void* image_proc_utils::ImageProviderShm::thread_function(void* lpParam) {
#endif


	ImageProviderShm * provider = (ImageProviderShm*)lpParam;


	std::vector<char> image;
	std::vector<char> header;

	while (provider->quit == false) {
		try {

			//prGetNext(std::vector<char> &image, std::vector<char> &header)
			image.resize(0);
			header.resize(0);
			provider->source->GetNext(image, header);
			if (header.size() > 0) {
				provider->callback(image, header);
			}
#ifdef _WIN32
			Sleep(provider->DURATION_MS);
#else
            usleep(provider->DURATION_MS*1000);
#endif            

		}

		catch (const exception& exp) {
			cout << exp.what() << endl;
			return 0;
		}
	}

#ifdef _WIN32
	return 0;
#else   
    return NULL;
#endif

}
