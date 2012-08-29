#ifndef CONSOLE

#include <v8.h>
#include <node.h>

#include "UsbDevice.h"
#include "protocol.h"

using namespace v8;
using namespace node;

UsbDevice *pDevice = NULL;

static Handle<Value> ping(const Arguments& args)
{
    return String::New("pong");
}

static Handle<Value> connect(const Arguments& args)
{
    if ( pDevice == NULL )
        pDevice = new UsbDevice();

    if( pDevice->ConnectToDevice() )
    {
        return String::New("Connected to device");
    }
    return String::New("Failed to connect");
}

static Handle<Value> disconnect(const Arguments& args)
{
    if ( pDevice != NULL )
    {
        pDevice->Dispose();
        delete pDevice;
        pDevice = NULL;
    }
    return True();
}

static Handle<Value> setTarget(const Arguments& args)
{
    if ( pDevice != NULL )
    {
        pDevice->SetTarget( args[0]->NumberValue(), args[1]->NumberValue() );
    }
    return True();
}

static Handle<Value> setVerbose(const Arguments& args)
{
    if ( pDevice == NULL )
        pDevice = new UsbDevice();

    if ( args.Length() > 0 ) {
        bool verbose = args[0]->BooleanValue();

        pDevice->SetVerbose( verbose );
    }

    return True();
}

extern "C" {
    static void init(Handle<Object> target)
    {
        NODE_SET_METHOD(target, "ping", ping);
        NODE_SET_METHOD(target, "connect", connect);
        NODE_SET_METHOD(target, "disconnect", disconnect);
        NODE_SET_METHOD(target, "setTarget", setTarget);
        NODE_SET_METHOD(target, "setVerbose", setVerbose);
    }

    NODE_MODULE(usbmaestro, init);
}

#endif
