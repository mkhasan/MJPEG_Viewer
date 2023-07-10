//#pragma once
#ifndef _IMAGE_PROVIDER_H
#define _IMAGE_PROVIDER_H

#include <functional>
#include <vector>


namespace image_proc_utils {
	
	class ImageProvider {
	public:
		
		typedef std::function<void(const std::vector<char>&, const std::vector<char>&)> Callback;
	protected:
		bool initialized;

		Callback callback;
	public:		
		
		ImageProvider() : initialized(false) {

		}


		virtual void Initialize(Callback _callback) {
			callback = _callback;
			initialized = true;
		};
		virtual void Finalize() {
			initialized = false;
		};
		virtual ~ImageProvider() {

		}

        bool IsInitialized() const {
            return initialized;
        }
	};

}
#endif
