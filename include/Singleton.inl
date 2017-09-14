#pragma once
#ifndef SINGLETON__H__3A24FF38_CCFA_4ADE_9490_A81932D07F82
#define SINGLETON__H__3A24FF38_CCFA_4ADE_9490_A81932D07F82

#include <boost/thread/recursive_mutex.hpp>

namespace Utils
{

    template<class TInstance> class Singleton
    {
    public:
        static TInstance * Instance();
        static void Destroy();
        static bool HasInstance();

    protected:
        Singleton();
        virtual ~Singleton();

    private:
        static boost::recursive_mutex   m_sync;
        static TInstance               *m_instance;
    };

    // ------------------------------------------------------------
    // ------------------------------------------------------------
    // --- implementation:
    template<class TInstance> boost::recursive_mutex Singleton<TInstance>::m_sync;
    template<class TInstance> TInstance * Singleton<TInstance>::m_instance(NULL);

    template <class TInstance> Singleton<TInstance>::Singleton()
    { }

    template <class TInstance> Singleton<TInstance>::~Singleton()
    { }

    template <class TInstance> TInstance* Singleton<TInstance>::Instance()
    {
        boost::recursive_mutex::scoped_lock lock(m_sync);
        if (m_instance == NULL)
        {
            m_instance = new TInstance();
        }

        TInstance *instance = m_instance;
        lock.unlock();

        return instance;
    }

    template <class TInstance> void Singleton<TInstance>::Destroy()
    {
        boost::recursive_mutex::scoped_lock lock(m_sync);
        if (m_instance != NULL)
        {
            delete m_instance;
            m_instance = NULL;
        }
    }

    template <class TInstance> bool Singleton<TInstance>::HasInstance()
    {
        boost::recursive_mutex::scoped_lock lock(m_sync);
        return m_instance != NULL;
    }

} // namespace Utils

#endif // SINGLETON__H__3A24FF38_CCFA_4ADE_9490_A81932D07F82
