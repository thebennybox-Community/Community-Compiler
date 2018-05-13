EXFN printf void [str i32]
EXFN assert void [i32]
INFN test void

PI32 16

LABL loop
	PI32 1
	ISUB

	DUPE
	PSTR "%X\n"
	CALL printf

	DUPE
	JGTZ loop
