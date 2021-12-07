#define ROTATE_SPEED 30
#define Ob_A Podstawa
#define Ob_B Ramie1
/*
 * Przykładowy zestaw poleceń
 */
Set Ob_A  0 -30 0 20 10 5  //Polozenie A
Set Ob_A.Ob_B 10 10 2 10 10 30 //Polozenie B
Rotate Ob_A.Ob_B ROTATE_SPEED Z 40
Rotate Ob_A ROTATE_SPEED Z 40
Pause Ob_A 1000 /*Zawieszenie na 1 sek. */
Move Ob_A 10 10 
Begin_Parallel_Actions
Rotate Ob_A ROTATE_SPEED Z 40/* Rotate i Move wykonywane razem */
Move Ob_A.Ob_B  10 10   
Rotate Ob_A.Ob_B.Ramie2 ROTATE_SPEED Z 40   
Move Ob_A.Ob_B.Ramie2  10 10    /* powoduje jazde po luku */
End_Parallel_Actions