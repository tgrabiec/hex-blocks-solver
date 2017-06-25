# hex-blocks-solver

Solver for *Block! Hexa Puzzle*: https://play.google.com/store/apps/details?id=com.bitmango.go.blockhexapuzzle

Currently preconfigured for one of the boards.

Sample output:

```
board:
     .---.     .---.     
.---.|   |.---.|   |.---.
|   |'___'|   |'___'|   |
'___'.---.'___'.---.'___'
.---.|   |.---.|   |.---.
|   |'___'|   |'___'|   |
'___'.---.'___'.---.'___'
.---.|   |.---.|   |.---.
|   |'___'|   |'___'|   |
'___'.---.'___'.---.'___'
.---.|   |.---.|   |.---.
|   |'___'|   |'___'|   |
'___'.---.'___'.---.'___'
.---.|   |.---.|   |.---.
|   |'___'|   |'___'|   |
'___'.---.'___'.---.'___'
.---.|   |.---.|   |.---.
|   |'___'|   |'___'|   |
'___'     '___'     '___'

shapes:
     
.---.
| 2 |
'___'

               
.---.     .---.
| 3 |     | 3 |
'___'.---.'___'
.---.| 3 |.---.
| 3 |'___'| 3 |
'___'     '___'

          
.---.     
| 4 |     
'___'.---.
     | 4 |
     '___'
          

                    
.---.     .---.     
| 5 |     | 5 |     
'___'.---.'___'.---.
     | 5 |.---.| 5 |
     '___'| 5 |'___'
          '___'     

               
.---.          
| 6 |          
'___'.---.     
.---.| 6 |.---.
| 6 |'___'| 6 |
'___'.---.'___'
     | 6 |     
     '___'     
               

     
.---.
| 7 |
'___'
.---.
| 7 |
'___'
.---.
| 7 |
'___'

               
          .---.
          | 8 |
     .---.'___'
.---.| 8 |     
| 8 |'___'     
'___'          
.---.          
| 8 |          
'___'          

     .---.     
.---.| 9 |     
| 9 |'___'     
'___'.---.     
     | 9 |.---.
     '___'| 9 |
          '___'

     .---.
.---.| A |
| A |'___'
'___'.---.
     | A |
     '___'
     .---.
.---.| A |
| A |'___'
'___'     

Performed 23979 steps in 0.139014 [s].
Solution found:
     .---.     .---.     
.---.| 5 |.---.| 5 |.---.
| 2 |'___'| 5 |'___'| 5 |
'___'.---.'___'.---.'___'
.---.| 4 |.---.| 5 |.---.
| 6 |'___'| 4 |'___'| A |
'___'.---.'___'.---.'___'
.---.| 6 |.---.| A |.---.
| 6 |'___'| 6 |'___'| A |
'___'.---.'___'.---.'___'
.---.| 6 |.---.| 8 |.---.
| 7 |'___'| 8 |'___'| A |
'___'.---.'___'.---.'___'
.---.| 8 |.---.| A |.---.
| 7 |'___'| 3 |'___'| 3 |
'___'.---.'___'.---.'___'
.---.| 8 |.---.| 3 |.---.
| 7 |'___'| 3 |'___'| 3 |
'___'     '___'     '___'

```