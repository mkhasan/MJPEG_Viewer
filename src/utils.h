/*
 * utils.h
 *
 *  Created on: Sep 4, 2020
 *      Author: hasan
 */

#ifndef SRC_UTILS_H_
#define SRC_UTILS_H_

#include <map>
#include <memory>
#include <iostream>
#include <vector>
#include <string>



//#define DETECTION_SENDER CustomDetectionSender
//#define DETECTION_SENDER MqttDetectionSender


namespace image_proc_utils {

#ifndef _WIN32

void print_time(char *);
const char * print_time();

#endif
std::vector<std::string> split(const std::string& s, char seperator);



class AbstractInstanceContainer
{
public:
    AbstractInstanceContainer() = default;
    virtual ~AbstractInstanceContainer() = default;

    AbstractInstanceContainer& operator=(AbstractInstanceContainer&& other) noexcept
    {
        mRawPointer = other.mRawPointer;
        other.mRawPointer = nullptr;

        std::cout << "AbstractInstanceContainer:: operator=" << std::endl;
        return *this;
    }

    void* get() {
        std::cout << "AbstractInstanceContainer::get()" << std::endl;
        return mRawPointer;
    }

protected:
    explicit AbstractInstanceContainer(void* ptr) : mRawPointer(ptr) {
        std::cout << "AbstractInstanceContainer" << std::endl;

    }

private:
    void* mRawPointer = nullptr;
};


template <class T>
class InstanceContainer : public AbstractInstanceContainer
{
public:
    explicit InstanceContainer(std::unique_ptr<T> ptr) : AbstractInstanceContainer(ptr.get()), mPointer(std::move(ptr)) {}
    ~InstanceContainer() override = default;

private:
    std::unique_ptr<T> mPointer;
};


class ServiceContainerException
{
public:
    explicit ServiceContainerException(std::string&& message) : message(message) {}

    std::string message;
};

class ServiceContainer
{
public:

    template <typename T, typename... Deps, typename... Args>
    void set(Args... args)
    {
        auto instance = std::make_unique<T>(get<typename std::remove_const<Deps>::type>()..., args...);
        std::unique_ptr<InstanceContainer<T>> service = std::make_unique<InstanceContainer<T>>(std::move(instance));
        mContainer[typeId<T>()] = std::move(service);
    }

    template <typename T>
    T* get()
    {
        auto it = mContainer.find(typeId<T>());
        if (it == mContainer.end()) {
            std::cout << std::string("Service '" + std::string(typeid(T).name()) + "' not registered in container.") << std::endl;
            throw ServiceContainerException(std::string("Service '" + std::string(typeid(T).name()) + "' not registered in container."));
        }
        //        assert (it != mContainer.end());

        return static_cast<T*>(it->second->get());
    }

private:
    template <typename T>
    int typeId()
    {
        static int id = ++mLastTypeId;

        return id;
    }

private:
    static int mLastTypeId;
    std::map<int, std::unique_ptr<AbstractInstanceContainer>> mContainer;

public:
    static ServiceContainer* Instance();
};

//int ServiceContainer::mLastTypeId = 0;




/*
#include <iostream>

struct Service1
{
    int alpha = 0;

    ~Service1() {
        std::cout << "Service 1 deleted" << std::endl;
    }
};


struct Service2
{
    explicit Service2(Service1* s1, int k) : s1(s1) {}      // "int k" is not present in the origin source

    int value() { return s1->alpha; }

    Service1* s1;

    ~Service2() {
        std::cout << "Service 2 deleted" << std::endl;
    }
};

struct Service3
{
    Service3(Service2* s2, Service1* s1)
    {

    }
};

int main ()
{

    {
        ServiceContainer serviceContainer;

        serviceContainer.set<Service1>();
        try {
            serviceContainer.set<Service3, Service2, Service1>();
        } catch (ServiceContainerException& e) {
            std::cerr << e.message << std::endl;
        }
        serviceContainer.set<Service2, Service1>(3);
        serviceContainer.set<Service3, Service2, Service1>();

        auto s1 = serviceContainer.get<Service1>();
        std::cout << s1->alpha << std::endl;
        s1->alpha = 42;
        std::cout << s1->alpha << std::endl;


        auto s2 = serviceContainer.get<Service2>();
        std::cout << s2->value() << std::endl;
        s1->alpha = 36;
        std::cout << s2->value() << std::endl;
        std::cout << serviceContainer.get<Service1>()->alpha << std::endl;
    }

    return 0;
}
 to join this conversation on GitHub. Al
*/




}



#endif /* SRC_UTILS_H_ */
