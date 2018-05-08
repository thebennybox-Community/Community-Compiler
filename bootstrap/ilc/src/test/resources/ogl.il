EXFN glutInit void []
EXFN glutInitDisplayMode void []
EXFN glutInitWindowSize void []
EXFN glutGet i32 []
EXFN glutInitWindowPosition void []
EXFN glutCreateWindow void []
EXFN glClearColor void []
EXFN glRotatef void []
EXFN glTranslatef void []
EXFN glColor3f void []
EXFN glVertex3f void []
EXFN glEnable void []
EXFN glClear void []
EXFN glBegin void []
EXFN gluPerspective void []
EXFN glutDisplayFunc void []
EXFN glutMainLoop void []
EXFN glEnd void []
EXFN glFlush void []
EXFN glutSwapBuffers void []
EXFN glutPostRedisplay void []

# EXFN glutInit void [ptr ptr]
# EXFN glutInitDisplayMode void [i32]
# EXFN glutInitWindowSize void [i32 i32]
# EXFN glutGet i32 [u32]
# EXFN glutInitWindowPosition void [i32 i32]
# EXFN glutCreateWindow void [str]
# EXFN glClearColor void [f32 f32 f32 f32]
# EXFN glRotatef void [f32 f32 f32 f32]
# EXFN glTranslatef void [f32 f32 f32]
# EXFN glColor3f void [f32 f32 f32]
# EXFN glVertex3f void [f32 f32 f32]
# EXFN glEnable void [u32]
# EXFN glClear void [u32]
# EXFN glBegin void [u32]
# EXFN gluPerspective void [f64 f64 f64 f64]
# EXFN glutDisplayFunc void [ptr]
# EXFN glutMainLoop void []
# EXFN glEnd void []
# EXFN glFlush void []
# EXFN glutSwapBuffers void []
# EXFN glutPostRedisplay void []

INFN display void []


LARG 4; ADRA 0; CALL glutInit
PI32 2; CALL glutInitDisplayMode
PI32 720; PI32 1280; CALL glutInitWindowSize

# Screen Height
PU32 201; CALL glutGet
PU32 720; ISUB
PU32 2; IDIV

PU32 200; CALL glutGet
PU32 1280; ISUB
PU32 2; IDIV

CALL glutInitWindowPosition

PSTR "OpenGL Test"; CALL glutCreateWindow
PF32 0.0; PF32 0.0; PF32 0.0; PF32 0.0; CALL glClearColor
PI32 2929; CALL glEnable; DELE
PF64 100.0; PF64 0.1; PF64 1.7777777777; PF64 85.0; CALL gluPerspective
PF32 -5; PF32 0; PF32 0; CALL glTranslatef
PFUN  display; CALL glutDisplayFunc
CALL glutMainLoop

RETN


FUNC display
	PI32 16384; PI32 256
	BWOR; CALL glClear

	PF32 0.5; PF32 0.75; PF32 1; PF32 1; CALL glRotatef

	PI32 7; CALL glBegin

	PF32 0.4; PF32 0.4; PF32 0.4; CALL glColor3f
	PF32 -1; PF32 -1; PF32 -1; CALL glVertex3f
	PF32  1; PF32 -1; PF32 -1; CALL glVertex3f
	PF32  1; PF32  1; PF32 -1; CALL glVertex3f
	PF32 -1; PF32  1; PF32 -1; CALL glVertex3f

	PF32 1; PF32 1; PF32 1; CALL glColor3f
	PF32 -1; PF32 -1; PF32 1; CALL glVertex3f
	PF32  1; PF32 -1; PF32 1; CALL glVertex3f
	PF32  1; PF32  1; PF32 1; CALL glVertex3f
	PF32 -1; PF32  1; PF32 1; CALL glVertex3f

	PF32 0.8; PF32 0.8; PF32 0.8; CALL glColor3f
	PF32 -1; PF32 -1; PF32 -1; CALL glVertex3f
	PF32 -1; PF32  1; PF32 -1; CALL glVertex3f
	PF32 -1; PF32  1; PF32  1; CALL glVertex3f
	PF32 -1; PF32 -1; PF32  1; CALL glVertex3f

	PF32 0.8; PF32 0.8; PF32 0.8; CALL glColor3f
	PF32 1; PF32 -1; PF32 -1; CALL glVertex3f
	PF32 1; PF32  1; PF32 -1; CALL glVertex3f
	PF32 1; PF32  1; PF32  1; CALL glVertex3f
	PF32 1; PF32 -1; PF32  1; CALL glVertex3f

	PF32 0.6; PF32 0.6; PF32 0.6; CALL glColor3f
	PF32 -1; PF32 -1; PF32 -1; CALL glVertex3f
	PF32  1; PF32 -1; PF32 -1; CALL glVertex3f
	PF32  1; PF32 -1; PF32  1; CALL glVertex3f
	PF32 -1; PF32 -1; PF32  1; CALL glVertex3f

	PF32 0.6; PF32 0.6; PF32 0.6; CALL glColor3f
	PF32 -1; PF32 1; PF32 -1; CALL glVertex3f
	PF32  1; PF32 1; PF32 -1; CALL glVertex3f
	PF32  1; PF32 1; PF32  1; CALL glVertex3f
	PF32 -1; PF32 1; PF32  1; CALL glVertex3f

	CALL glEnd
	CALL glFlush
	CALL glutSwapBuffers
	CALL glutPostRedisplay

	RETN