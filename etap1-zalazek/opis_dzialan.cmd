#define ROTATE_SPEED 30
#define Ob_A Podstawa
#define Ob_B Ramie1
/*
 * Przykładowy zestaw poleceń
 */
Set Ob_A 2 0 30 10 40 //Polozenie A
Set Ob_A.Ob_B 10 10 0 20 55 //Polozenie B
Rotate Ob_A.Ob_B ROTATE_SPEED Z 40
Pause Ob_A 1000 /*Zawieszenie na 1 sek. */
Move Ob_A 10 10 
Rotate Ob_A ROTATE_SPEED Z 60 /* Rotate i Move wykonywane razem */
Move Ob_A.Ob_B 10 10        /* powoduje jazde po luku */