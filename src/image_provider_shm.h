#ifndef _IMAGE_PROVIDER_SHM_H
#define _IMAGE_PROVIDER_SHM_H

#include "image_provider.h"
#include "ImageSource.h"

#ifdef _WIN32
#include <windows.h>
#else 
#include<pthread.h>
#include <unistd.h>
#endif

namespace image_proc_utils {

	class ImageProviderShm : public ImageProvider {

		const int DURATION_MS = 100;
		ImageSource* source;
#ifdef _WIN32
		HANDLE  hThread;
#else
        pthread_t t_id;
#endif

		bool quit;
		

	public:
		

		ImageProviderShm(ImageSource* source);
		virtual ~ImageProviderShm();

		virtual void Initialize(Callback callback) override;
		virtual void Finalize() override;
#ifdef _WIN32
		static DWORD WINAPI ThreadFunction(LPVOID lpParam);
#else
        static void* thread_function(void* arg);        
#endif

	};

}
#endif

