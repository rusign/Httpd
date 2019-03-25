#pragma once

#include <unistd.h>
#include <sys/syscall.h>
#include "httpEngine/Engine.hh"
#include "network/ISocket.h"

namespace test
{
    class SocketImplTest : public zia::ISocket
    {
    public:
        void connect() override {}
        ISocket *accept() const override { return nullptr; }

        apouche::IZiaConnection::NativeHandle getNativeHandle() const { return 0; }
        int ReceiveRequest() { return 0; }
        int SendResponse(apouche::IHttpResponse *) { return 0; }
        std::vector<apouche::IHttpRequest *> const &getRequests() const { return _requests; }
        apouche::IHttpRequest *getNextRequest() { return nullptr; }

        virtual bool processing() const { return false; }
        virtual bool disconnected() const { return false; }
        virtual void process() {}
        virtual void unprocess() {}
        virtual void disconnect() {}

    private:
        void bind() override {}
        void listen(int) override {}

    private:
        std::vector<apouche::IHttpRequest *> _requests;
    };

    class EngineContainer : public zia::ISocket
    {
    public:
        EngineContainer(zia::httpEngine::Engine *e) : engine_(e) {}

        void connect() override {}
        ISocket *accept() const override { return nullptr; }

        apouche::IZiaConnection::NativeHandle getNativeHandle() const { return 0; }
        int ReceiveRequest() { return 0; }
        int SendResponse(apouche::IHttpResponse *) { return 0; }
        std::vector<apouche::IHttpRequest *> const &getRequests() const { return requests_; }
        apouche::IHttpRequest *getNextRequest() { return nullptr; }

        int getId() const { return syscall(SYS_gettid); }
        const zia::httpEngine::Engine *getEngine() const { return engine_; }

    private:
        void bind() override {}
        void listen(int) override {}

    private:
        std::vector<apouche::IHttpRequest *> requests_;
        zia::httpEngine::Engine              *engine_;
    };
}