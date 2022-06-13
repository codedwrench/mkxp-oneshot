#include "binding-util.h"
#include "binding-types.h"
#include "sharedstate.h"
#include "eventthread.h"
#include "otherview-message.h"
#include "debugwriter.h"

RB_METHOD(sendMessage)
{
    OtherViewMessager &messager = shState->otherView();

    const char* message;
    rb_get_args(argc, argv, "z", &message RB_ARG_END);
    Debug() << "Sending message: " << message;

    bool ret = messager.sendMsg(message);

    return rb_bool_new(ret);
}
RB_METHOD(readMessage) 
{
    OtherViewMessager &messager = shState->otherView();
    VALUE rtn = rb_str_new_cstr(messager.getMsg().c_str());
    return rtn;
}


void otherviewBindingInit()
{
    VALUE module = rb_define_module("OtherView");
    _rb_define_module_function(module, "send", sendMessage);
    _rb_define_module_function(module, "read", readMessage);
}