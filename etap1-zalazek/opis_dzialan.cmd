#define ROTATE_SPEED 1
#define Ob_A Podstawa
#define Ob_B Podstawa2
/*
 * Przykładowy zestaw poleceń
 */
Rotate Ob_A ROTATE_SPEED Z 20
Move Ob_A 0.5 5

Move Ob_B 0.2 8
Move Ob_B -0.2 8
Begin_Parallel_Actions
Rotate Ob_A -ROTATE_SPEED Z 90
Move Ob_A 0.1 5
Rotate Ob_B ROTATE_SPEED Z 90
Move Ob_B 0.1 9
End_Parallel_Actions