/*skynet是基于多线程的，每个actor都会被单独的线程调度，且每个actor可以杀死其它actor,给其它actor发送消息，创建actor，也就是一个actor可能被多个线程持有，那么就会面临三个问题：

一个actor被同时使用时，如何安全释放。
actor被释放后，外部使用时如何检测该actor已经无效了，以便流程能继续。
若信箱里的消息具有请求回应语义，那么如果通知消息源。
框架使用的是handle映射与引用计数的手法

如果没有gc,那么在多线程编程中，如何安全释放资源是一定会面临的问题。通常将它独立到另外的模块中解决，有两种常用的方法：

 本文的handle映射和引用计数。c++中通常用智能指针，通过析构、拷贝构造函数自动来加减引用计数做强制保证。个人觉得前者更为灵活。
释放时只打上标记，以一定频率定时回收资源。
*/


#ifndef SKYNET_CONTEXT_HANDLE_H
#define SKYNET_CONTEXT_HANDLE_H

#include <stdint.h>

// reserve high 8 bits for remote id
#define HANDLE_MASK 0xffffff
#define HANDLE_REMOTE_SHIFT 24

struct skynet_context;

uint32_t skynet_handle_register(struct skynet_context *);
int skynet_handle_retire(uint32_t handle);
struct skynet_context * skynet_handle_grab(uint32_t handle);
void skynet_handle_retireall();

uint32_t skynet_handle_findname(const char * name);
const char * skynet_handle_namehandle(uint32_t handle, const char *name);

void skynet_handle_init(int harbor);

#endif
