local skynet = require "skynet"
local coroutine = coroutine
local xpcall = xpcall
local traceback = debug.traceback
local table = table

function skynet.queue()
	local current_thread
	local ref = 0
	local thread_queue = {}

	local function xpcall_ret(ok, ...)
		ref = ref - 1
		if ref == 0 then
			current_thread = table.remove(thread_queue,1)
			if current_thread then
				-- 唤醒被让出执行流的协程，与skynet.wait()相对应
				skynet.wakeup(current_thread)
			end
		end
		assert(ok, (...))
		return ...
	end

	return function(f, ...)
		local thread = coroutine.running()
		-- 如果启动了新的协程，保存并让出该执行流
		if current_thread and current_thread ~= thread then
			table.insert(thread_queue, thread)
			skynet.wait()
			assert(ref == 0)	-- current_thread == thread
		end
		current_thread = thread

		ref = ref + 1
		return xpcall_ret(xpcall(f, traceback, ...))
	end
end

return skynet.queue
