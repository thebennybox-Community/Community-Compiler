
add: i32 i32 i32

load.arg 0
store.loc 0

load.arg 4
store.loc 4

load.loc 0
load.loc 4

Add

ret

test: 
push.str "hello world!"
fcall printf

ret
