#ifndef _GENERATE_H
#define _GENERATE_H

#define	MAXPROD	100
#define	MAXAXIOM 100
#define	MAXSTR 100000
#define	MAXCHARS 256
#define	MAXIGNORE 100

typedef struct Parameter {
	char	*filename;
	int	n;
	int	angle;
	int	scale;
}Parameter;
typedef struct {
	char	*lCon;
	int	lConLen;
	char	*pred;
	int	predLen;
	char	*rCon;
	int	rConLen;
	char	*succ;
	int	succLen;
}Production;
/* note this has changed from Pair which is now a vector2 definition */
typedef struct {
	float	x,y;
	int	colour;
}GOEL_pair;


#endif /* _GENERATE_H */
