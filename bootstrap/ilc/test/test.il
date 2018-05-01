
start:

push.str ""

push.u32 0
store.loc 0
addr.loc 0

fcall glutInit u32 str

push.i32 0
fcall glutInitDisplayMode i32

push.i32 600
push.i32 800
fcall glutInitWindowSize i32 i32

push.str "Test"
fcall glutCreateWindow str

push.f32 0.0
push.f32 0.0
push.f32 0.0
push.f32 0.0
fcall glClearColor

push.fn display
fcall glutDisplayFunc
  
fcall glutMainLoop

push.i32 0
ret

display:

push.i32 16384
fcall glClear i32

push.f32 1.0
push.f32 1.0
push.f32 1.0
fcall glColor3f f32 f32 f32

push.i32 4
fcall glBegin		

push.f32 -0.5
push.f32 -0.5
fcall glVertex2f 
    
push.f32 -0.5
push.f32 0.5
fcall glVertex2f
    
push.f32 0.5
push.f32 0
fcall glVertex2f

fcall glEnd	
fcall glFlush
ret