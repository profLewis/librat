#ifndef _GENERATE_H
#define _GENERATE_H

#define	MAXPROD	100
#define	MAXAXIOM 100
#define	MAXSTR 100000
#define	MAXCHARS 256
#define	MAXIGNORE 100

typedef struct Parameter {
	char	*filename;
	INT	n;
	INT	angle;
	INT	scale;
}Parameter;
typedef struct {
	char	*lCon;
	INT	lConLen;
	char	*pred;
	INT	predLen;
	char	*rCon;
	INT	rConLen;
	char	*succ;
	INT	succLen;
}Production;
/* note this has changed from Pair which is now a vector2 definition */
typedef struct {
	FLOAT	x,y;
	INT	colour;
}GOEL_PAIR;


#endif /* _GENERATE_H */
