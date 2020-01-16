extensions [ py ]

globals [ DNAs new_DNAs ]

breed [ preys prey ]
preys-own [
            Dimension
            Voracity        ;how fast he eats
            Vision          ;how far he sees
            Pleasantness    ;ininfluent parameter
            Capacity        ;capacity of store energy

            DNA             ;list containing the genetic parameters

            Energy
            Energy_cost
            Max_energy
            Life_time
            Fitness_on_time
            Fitness

           ]

breed [ predators predator ]
predators-own [
                Dimension
                Vision
              ]


patches-own [
              fertile?
              countdown
              resource
              max_resource

            ]


to setup

  clear-all
  reset-ticks

  ask patches [
                ifelse ( random 100 <= Fertility  ) [ set fertile? True  ]
                                                    [ set fertile? False ]            ;if 0 not fertile

                ifelse fertile? [
                                  set countdown random grass-regrowth-time + 10       ;initialize grass regrowth clocks randomly (minimo 10)
                                  set resource random environment_prosperity + 1      ;initialize the amount of grass randomly
                                  set max_resource random max_grass + 1
                                  set pcolor green

                                ]

                                [
                                  set resource 0
                                ]

               ]



  ask patches [ paint_grass ]


  populate_with_preys      ;create preys

  set DNAs []

  create-predators predators_number [
                                      setxy random-pxcor random-pycor
                                      set Dimension Predators_dimension
                                      set Vision Predators_vision
                                      set color black
                                      set size 2

                                      set shape "wolf"
                                    ]


  py:setup "python3"
  py:run "import genetic_algorithm as g_a"


  tick


end




to populate_with_preys

  create-preys preys_number [                                             ;create preys with different features
                               setxy random-pxcor random-pycor

                               set Dimension random 50 + 1
                               set Voracity random 50 + 1
                               set Vision random 50 + 1
                               set Pleasantness random 50 + 1
                               set Capacity random 50 + 1
                               set size 2

                               set Max_energy ( Capacity * Voracity + 1 )
                               set Energy ( Max_energy / 4 )
                               set Energy_cost ( Dimension + Voracity + ( Capacity / 5 ) )
                               set Life_time 1
                               set Fitness 0
                               set Fitness_on_time 0
                               set color white

                               set DNA []                      ;inserisco le caratteristiche in una lista
                               set DNA lput Dimension DNA
                               set DNA lput Voracity DNA
                               set DNA lput Vision DNA
                               set DNA lput Pleasantness DNA
                               set DNA lput Capacity DNA
                               set DNA lput Fitness_on_time DNA


                               set shape "sheep"
                             ]

end




to go

  ask preys [
              act
              set Fitness (Fitness + Energy / Max_energy)
              set Life_time Life_time + 1

              set Fitness_on_time (Fitness / Life_time)
            ]

  ask predators [
                  let p one-of neighbors
                  face p
                  move-to p

                  let victim one-of preys in-radius Vision
                  if victim != nobody [
                                        let Dimension_of_predator Dimension
                                        ask prey [who] of victim [
                                                                   if (Dimension / Dimension_of_predator) < random-float 1 [die write "I'm dead"]
                                                                 ]

                                       ]

                ]


  grow        ;manages the re-growth of resources



  ask preys [
               set DNA but-last DNA      ;remove the fitness and reinsert it updated
               set DNA lput Fitness_on_time DNA

               set DNAs lput DNA DNAs     ;create a list of genetic
            ]

  ifelse count preys > 1 [reproduce]      ;at least a couple is needed for reproduction
                         [
                           write "EXTINTION"
                           stop
                         ]



  set DNAs []         ;IMPORTANT: eliminates the DNA of previous species


  tick


end


to act

  let n_enemies count predators in-radius Vision

  ifelse n_enemies > 0 [
                         let enemy one-of predators in-radius Vision
                         face enemy
                         set heading ( heading + 180 )
                         set Energy ( Energy - Energy_cost )
                         if Energy < 0 [ die ]
                         forward 1
                       ]

                       [
                          ifelse resource > 0 and Energy < Max_energy [ feed ]
                                                                      [ move ]
                       ]

end


to feed

  let max_meal Voracity      ;max amount of food he can eat

  ifelse resource >= max_meal [
                                set Energy ( Energy + max_meal )
                                set resource ( resource - max_meal )
                              ]

                              [
                                set Energy ( Energy + resource )
                                set resource 0
                              ]

  if Energy > Max_energy [ set Energy Max_energy ]


end


to move

  set Energy ( Energy -  Energy_cost )

  if Energy <= 0 [
                   die
                 ]

 move-to patch-here                        ; go to patch center
 let p max-one-of neighbors [resource]     ; or neighbors4
 if [resource] of p >= resource [
                                 face p
                                 move-to p
                                ]

end


to grow

  ask patches [
                paint_grass
                if fertile? and ( (ticks mod countdown) = 0 ) and resource < max_resource [
                                                                                            set resource ( resource + random environment_prosperity )
                                                                                            if resource > max_resource [ set resource max_resource ]
                                                                                          ]
              ]



end


to paint_grass

  if resource >= ( max_grass / 2 ) [ set pcolor 54 ]
  if resource < ( max_grass / 2 ) and resource >= ( max_grass / 4 ) [ set pcolor 55 ]
  if resource < ( max_grass / 4 ) and resource >= ( max_grass / 8 ) [ set pcolor 56 ]
  if resource < ( max_grass / 8 ) and resource >= ( max_grass / 16 )[ set pcolor 57 ]
  if resource < ( max_grass / 16 ) and resource > 0 [ set pcolor 58 ]
  if resource = 0 [ set pcolor 47 ]


end

to reproduce

  py:set "DNAs" DNAs
  py:set "prob_mutation" prob_mutation      ;mutation probability
  ;show py:runresult "DNAs"

  set new_DNAs py:runresult "g_a.genetic_algorithm( DNAs, prob_mutation )"

  let new_DNA1 item 0 new_DNAs
  let new_DNA2 item 1 new_DNAs

  create-preys 1 [                                             ;create preys with different features
                   setxy random-pxcor random-pycor

                   set Dimension item 0 new_DNA1
                   set Voracity item 1 new_DNA1
                   set Vision item 2 new_DNA1
                   set Pleasantness item 3 new_DNA1
                   set Capacity item 4 new_DNA1
                   set size 2

                   set Max_energy ( Capacity * Voracity + 1 )
                   set Energy ( Max_energy / 4 )
                   set Energy_cost ( Dimension + Voracity + (Capacity / 5) )
                   set Life_time 1
                   set Fitness 0
                   set Fitness_on_time 0
                   set color white


                   set DNA []
                   set DNA lput Dimension DNA
                   set DNA lput Voracity DNA
                   set DNA lput Vision DNA
                   set DNA lput Pleasantness DNA
                   set DNA lput Capacity DNA
                   set DNA lput Fitness_on_time DNA


                   set shape "sheep"

                 ]

   create-preys 1 [                                             ;create preys with different features
                   setxy random-pxcor random-pycor

                   ;set Speed random 5 + 1
                   set Dimension item 0 new_DNA2
                   set Voracity item 1 new_DNA2
                   set Vision item 2 new_DNA2
                   set Pleasantness item 3 new_DNA2
                   set Capacity item 4 new_DNA2
                   set size 2

                   set Max_energy ( Capacity * Voracity + 1 )
                   set Energy ( Max_energy / 4 )
                   set Energy_cost ( Dimension + Voracity + (Capacity / 5) )
                   set Life_time 1
                   set Fitness 0
                   set Fitness_on_time 0
                   set color white

                   set DNA []
                   set DNA lput Dimension DNA
                   set DNA lput Voracity DNA
                   set DNA lput Vision DNA
                   set DNA lput Pleasantness DNA
                   set DNA lput Capacity DNA
                   set DNA lput Fitness_on_time DNA


                   set shape "sheep"
                 ]



end
@#$#@#$#@
GRAPHICS-WINDOW
183
10
545
373
-1
-1
10.73
1
10
1
1
1
0
1
1
1
-16
16
-16
16
0
0
1
ticks
30.0

BUTTON
60
42
138
75
set up
setup
NIL
1
T
OBSERVER
NIL
NIL
NIL
NIL
1

SLIDER
595
58
797
91
grass-regrowth-time
grass-regrowth-time
0
100
10.0
10
1
NIL
HORIZONTAL

SLIDER
598
228
808
261
preys_number
preys_number
0
200
200.0
2
1
NIL
HORIZONTAL

BUTTON
60
92
123
125
go
go
T
1
T
OBSERVER
NIL
NIL
NIL
NIL
1

SLIDER
596
18
768
51
fertility
fertility
0
100
100.0
1
1
NIL
HORIZONTAL

SLIDER
595
105
821
138
environment_prosperity
environment_prosperity
10
100
60.0
5
1
NIL
HORIZONTAL

SLIDER
595
154
767
187
max_grass
max_grass
10
100
35.0
5
1
NIL
HORIZONTAL

SLIDER
860
229
1050
262
predators_number
predators_number
0
50
10.0
1
1
NIL
HORIZONTAL

PLOT
930
10
1295
187
Number of preys
time
NIL
0.0
10.0
0.0
10.0
true
false
"" ""
PENS
"default" 1.0 0 -13791810 true "" "if ticks > 1 [plot count preys]"

PLOT
1146
199
1527
404
Mean Fitness
time
NIL
0.0
10.0
0.0
1.0
true
false
"" ""
PENS
"default" 1.0 0 -2674135 true "" "if ticks > 1 [plot mean [Fitness_on_time] of preys]"

PLOT
1342
10
1714
193
Mean life time
time
NIL
0.0
10.0
0.0
13.0
true
false
"" ""
PENS
"default" 1.0 0 -5825686 true "" "if ticks > 1 [plot mean [Life_time] of preys]"

PLOT
36
425
392
611
Mean Vision
time
mean vision
0.0
10.0
0.0
10.0
true
false
"" ""
PENS
"default" 1.0 0 -13840069 true "" "if ticks > 1 [plot mean [Vision] of preys]"

PLOT
419
424
773
611
Mean Dimension
time
mean dimension
0.0
10.0
0.0
10.0
true
false
"" ""
PENS
"default" 1.0 0 -1184463 true "" "if ticks > 1 [plot mean [Dimension] of preys]"

PLOT
801
423
1152
611
Mean Voracity
time
mean voracity
0.0
10.0
0.0
10.0
true
false
"" ""
PENS
"default" 1.0 0 -11033397 true "" "if ticks > 1 [plot mean [Voracity] of preys]"

SLIDER
599
270
771
303
prob_mutation
prob_mutation
0
100
7.0
1
1
NIL
HORIZONTAL

PLOT
1563
421
1892
611
Mean Pleasantness
time
m. pleasantness
0.0
10.0
0.0
10.0
true
false
"" ""
PENS
"default" 1.0 0 -955883 true "" "if ticks > 1 [plot mean [Pleasantness] of preys]"

PLOT
33
619
391
782
Vision variance
time
variance
0.0
10.0
0.0
10.0
true
false
"" ""
PENS
"default" 1.0 0 -2674135 true "" "if ticks > 1 [plot variance [Vision] of preys]"

PLOT
1562
620
1895
777
Pleasantness variance
time
variance
0.0
10.0
0.0
10.0
true
false
"" ""
PENS
"default" 1.0 0 -2674135 true "" "if ticks > 1 [plot variance [Pleasantness] of preys]"

PLOT
1168
423
1537
610
Mean Capacity
time
mean capacity
0.0
10.0
0.0
10.0
true
false
"" ""
PENS
"default" 1.0 0 -6459832 true "" "if ticks > 1 [plot mean [Capacity] of preys]"

PLOT
417
621
773
784
Dimension variance
time
variance
0.0
10.0
0.0
10.0
true
false
"" ""
PENS
"default" 1.0 0 -2674135 true "" "if ticks > 1 [plot variance [Dimension] of preys]"

PLOT
801
620
1153
780
Voracity variance
time
variance
0.0
10.0
0.0
10.0
true
false
"" ""
PENS
"default" 1.0 0 -2674135 true "" "if ticks > 1 [plot variance [Voracity] of preys]"

PLOT
1169
621
1539
780
Capacity variance
time
variance
0.0
10.0
0.0
10.0
true
false
"" ""
PENS
"default" 1.0 0 -2674135 true "" "if ticks > 1 [plot variance [Capacity] of preys]"

SLIDER
861
268
1074
301
predators_dimension
predators_dimension
1
50
50.0
1
1
NIL
HORIZONTAL

SLIDER
860
310
1035
343
predators_vision
predators_vision
0
10
3.0
1
1
NIL
HORIZONTAL

@#$#@#$#@
## WHAT IS IT?

The aim of the work is to reproduce the evolution process through an agentbased simulation. We built a prey-predator model in which preys are characterized by parameters that determine their survival in the simulated environment. Preys feed themselves with resources provided by the environment and try to escape from predators. They reproduce generating individuals whose parameters are a combination of those of parents. Genetic algorithms (written in Python) regulate the entire reproductive process by fostering the reproduction of the most suitable individuals. Due to this model we expect that the parameters of next generations will converge to the best choice for the survival.

## HOW IT WORKS

It is possible to find all the details of the work in this paper (inserire link)
## HOW TO USE IT

(how to use the model, including a description of each of the items in the Interface tab)

## THINGS TO NOTICE

(suggested things for the user to notice while running the model)

## THINGS TO TRY

Run the model with different parameters in order to observe the different evolutionary choice of the preys.

## EXTENDING THE MODEL

The model can be improved in several ways, for example we could use
continuous genetic parameters instead of discrete ones. Furthermore, making
the predators more complex and able to reproduce would be interesting.
This would allow us to observe the process of coevolution between prey and
predator.

## NETLOGO FEATURES

(interesting or unusual features of NetLogo that the model uses, particularly in the Code tab; or where workarounds were needed for missing features)

## RELATED MODELS

(models in the NetLogo Models Library and elsewhere which are of related interest)

## CREDITS AND REFERENCES

(a reference to the model's URL on the web if it has one, as well as any other necessary credits, citations, and links)
@#$#@#$#@
default
true
0
Polygon -7500403 true true 150 5 40 250 150 205 260 250

airplane
true
0
Polygon -7500403 true true 150 0 135 15 120 60 120 105 15 165 15 195 120 180 135 240 105 270 120 285 150 270 180 285 210 270 165 240 180 180 285 195 285 165 180 105 180 60 165 15

arrow
true
0
Polygon -7500403 true true 150 0 0 150 105 150 105 293 195 293 195 150 300 150

box
false
0
Polygon -7500403 true true 150 285 285 225 285 75 150 135
Polygon -7500403 true true 150 135 15 75 150 15 285 75
Polygon -7500403 true true 15 75 15 225 150 285 150 135
Line -16777216 false 150 285 150 135
Line -16777216 false 150 135 15 75
Line -16777216 false 150 135 285 75

bug
true
0
Circle -7500403 true true 96 182 108
Circle -7500403 true true 110 127 80
Circle -7500403 true true 110 75 80
Line -7500403 true 150 100 80 30
Line -7500403 true 150 100 220 30

butterfly
true
0
Polygon -7500403 true true 150 165 209 199 225 225 225 255 195 270 165 255 150 240
Polygon -7500403 true true 150 165 89 198 75 225 75 255 105 270 135 255 150 240
Polygon -7500403 true true 139 148 100 105 55 90 25 90 10 105 10 135 25 180 40 195 85 194 139 163
Polygon -7500403 true true 162 150 200 105 245 90 275 90 290 105 290 135 275 180 260 195 215 195 162 165
Polygon -16777216 true false 150 255 135 225 120 150 135 120 150 105 165 120 180 150 165 225
Circle -16777216 true false 135 90 30
Line -16777216 false 150 105 195 60
Line -16777216 false 150 105 105 60

car
false
0
Polygon -7500403 true true 300 180 279 164 261 144 240 135 226 132 213 106 203 84 185 63 159 50 135 50 75 60 0 150 0 165 0 225 300 225 300 180
Circle -16777216 true false 180 180 90
Circle -16777216 true false 30 180 90
Polygon -16777216 true false 162 80 132 78 134 135 209 135 194 105 189 96 180 89
Circle -7500403 true true 47 195 58
Circle -7500403 true true 195 195 58

circle
false
0
Circle -7500403 true true 0 0 300

circle 2
false
0
Circle -7500403 true true 0 0 300
Circle -16777216 true false 30 30 240

cow
false
0
Polygon -7500403 true true 200 193 197 249 179 249 177 196 166 187 140 189 93 191 78 179 72 211 49 209 48 181 37 149 25 120 25 89 45 72 103 84 179 75 198 76 252 64 272 81 293 103 285 121 255 121 242 118 224 167
Polygon -7500403 true true 73 210 86 251 62 249 48 208
Polygon -7500403 true true 25 114 16 195 9 204 23 213 25 200 39 123

cylinder
false
0
Circle -7500403 true true 0 0 300

dot
false
0
Circle -7500403 true true 90 90 120

face happy
false
0
Circle -7500403 true true 8 8 285
Circle -16777216 true false 60 75 60
Circle -16777216 true false 180 75 60
Polygon -16777216 true false 150 255 90 239 62 213 47 191 67 179 90 203 109 218 150 225 192 218 210 203 227 181 251 194 236 217 212 240

face neutral
false
0
Circle -7500403 true true 8 7 285
Circle -16777216 true false 60 75 60
Circle -16777216 true false 180 75 60
Rectangle -16777216 true false 60 195 240 225

face sad
false
0
Circle -7500403 true true 8 8 285
Circle -16777216 true false 60 75 60
Circle -16777216 true false 180 75 60
Polygon -16777216 true false 150 168 90 184 62 210 47 232 67 244 90 220 109 205 150 198 192 205 210 220 227 242 251 229 236 206 212 183

fish
false
0
Polygon -1 true false 44 131 21 87 15 86 0 120 15 150 0 180 13 214 20 212 45 166
Polygon -1 true false 135 195 119 235 95 218 76 210 46 204 60 165
Polygon -1 true false 75 45 83 77 71 103 86 114 166 78 135 60
Polygon -7500403 true true 30 136 151 77 226 81 280 119 292 146 292 160 287 170 270 195 195 210 151 212 30 166
Circle -16777216 true false 215 106 30

flag
false
0
Rectangle -7500403 true true 60 15 75 300
Polygon -7500403 true true 90 150 270 90 90 30
Line -7500403 true 75 135 90 135
Line -7500403 true 75 45 90 45

flower
false
0
Polygon -10899396 true false 135 120 165 165 180 210 180 240 150 300 165 300 195 240 195 195 165 135
Circle -7500403 true true 85 132 38
Circle -7500403 true true 130 147 38
Circle -7500403 true true 192 85 38
Circle -7500403 true true 85 40 38
Circle -7500403 true true 177 40 38
Circle -7500403 true true 177 132 38
Circle -7500403 true true 70 85 38
Circle -7500403 true true 130 25 38
Circle -7500403 true true 96 51 108
Circle -16777216 true false 113 68 74
Polygon -10899396 true false 189 233 219 188 249 173 279 188 234 218
Polygon -10899396 true false 180 255 150 210 105 210 75 240 135 240

house
false
0
Rectangle -7500403 true true 45 120 255 285
Rectangle -16777216 true false 120 210 180 285
Polygon -7500403 true true 15 120 150 15 285 120
Line -16777216 false 30 120 270 120

leaf
false
0
Polygon -7500403 true true 150 210 135 195 120 210 60 210 30 195 60 180 60 165 15 135 30 120 15 105 40 104 45 90 60 90 90 105 105 120 120 120 105 60 120 60 135 30 150 15 165 30 180 60 195 60 180 120 195 120 210 105 240 90 255 90 263 104 285 105 270 120 285 135 240 165 240 180 270 195 240 210 180 210 165 195
Polygon -7500403 true true 135 195 135 240 120 255 105 255 105 285 135 285 165 240 165 195

line
true
0
Line -7500403 true 150 0 150 300

line half
true
0
Line -7500403 true 150 0 150 150

pentagon
false
0
Polygon -7500403 true true 150 15 15 120 60 285 240 285 285 120

person
false
0
Circle -7500403 true true 110 5 80
Polygon -7500403 true true 105 90 120 195 90 285 105 300 135 300 150 225 165 300 195 300 210 285 180 195 195 90
Rectangle -7500403 true true 127 79 172 94
Polygon -7500403 true true 195 90 240 150 225 180 165 105
Polygon -7500403 true true 105 90 60 150 75 180 135 105

plant
false
0
Rectangle -7500403 true true 135 90 165 300
Polygon -7500403 true true 135 255 90 210 45 195 75 255 135 285
Polygon -7500403 true true 165 255 210 210 255 195 225 255 165 285
Polygon -7500403 true true 135 180 90 135 45 120 75 180 135 210
Polygon -7500403 true true 165 180 165 210 225 180 255 120 210 135
Polygon -7500403 true true 135 105 90 60 45 45 75 105 135 135
Polygon -7500403 true true 165 105 165 135 225 105 255 45 210 60
Polygon -7500403 true true 135 90 120 45 150 15 180 45 165 90

sheep
false
15
Circle -1 true true 203 65 88
Circle -1 true true 70 65 162
Circle -1 true true 150 105 120
Polygon -7500403 true false 218 120 240 165 255 165 278 120
Circle -7500403 true false 214 72 67
Rectangle -1 true true 164 223 179 298
Polygon -1 true true 45 285 30 285 30 240 15 195 45 210
Circle -1 true true 3 83 150
Rectangle -1 true true 65 221 80 296
Polygon -1 true true 195 285 210 285 210 240 240 210 195 210
Polygon -7500403 true false 276 85 285 105 302 99 294 83
Polygon -7500403 true false 219 85 210 105 193 99 201 83

square
false
0
Rectangle -7500403 true true 30 30 270 270

square 2
false
0
Rectangle -7500403 true true 30 30 270 270
Rectangle -16777216 true false 60 60 240 240

star
false
0
Polygon -7500403 true true 151 1 185 108 298 108 207 175 242 282 151 216 59 282 94 175 3 108 116 108

target
false
0
Circle -7500403 true true 0 0 300
Circle -16777216 true false 30 30 240
Circle -7500403 true true 60 60 180
Circle -16777216 true false 90 90 120
Circle -7500403 true true 120 120 60

tree
false
0
Circle -7500403 true true 118 3 94
Rectangle -6459832 true false 120 195 180 300
Circle -7500403 true true 65 21 108
Circle -7500403 true true 116 41 127
Circle -7500403 true true 45 90 120
Circle -7500403 true true 104 74 152

triangle
false
0
Polygon -7500403 true true 150 30 15 255 285 255

triangle 2
false
0
Polygon -7500403 true true 150 30 15 255 285 255
Polygon -16777216 true false 151 99 225 223 75 224

truck
false
0
Rectangle -7500403 true true 4 45 195 187
Polygon -7500403 true true 296 193 296 150 259 134 244 104 208 104 207 194
Rectangle -1 true false 195 60 195 105
Polygon -16777216 true false 238 112 252 141 219 141 218 112
Circle -16777216 true false 234 174 42
Rectangle -7500403 true true 181 185 214 194
Circle -16777216 true false 144 174 42
Circle -16777216 true false 24 174 42
Circle -7500403 false true 24 174 42
Circle -7500403 false true 144 174 42
Circle -7500403 false true 234 174 42

turtle
true
0
Polygon -10899396 true false 215 204 240 233 246 254 228 266 215 252 193 210
Polygon -10899396 true false 195 90 225 75 245 75 260 89 269 108 261 124 240 105 225 105 210 105
Polygon -10899396 true false 105 90 75 75 55 75 40 89 31 108 39 124 60 105 75 105 90 105
Polygon -10899396 true false 132 85 134 64 107 51 108 17 150 2 192 18 192 52 169 65 172 87
Polygon -10899396 true false 85 204 60 233 54 254 72 266 85 252 107 210
Polygon -7500403 true true 119 75 179 75 209 101 224 135 220 225 175 261 128 261 81 224 74 135 88 99

wheel
false
0
Circle -7500403 true true 3 3 294
Circle -16777216 true false 30 30 240
Line -7500403 true 150 285 150 15
Line -7500403 true 15 150 285 150
Circle -7500403 true true 120 120 60
Line -7500403 true 216 40 79 269
Line -7500403 true 40 84 269 221
Line -7500403 true 40 216 269 79
Line -7500403 true 84 40 221 269

wolf
false
0
Polygon -16777216 true false 253 133 245 131 245 133
Polygon -7500403 true true 2 194 13 197 30 191 38 193 38 205 20 226 20 257 27 265 38 266 40 260 31 253 31 230 60 206 68 198 75 209 66 228 65 243 82 261 84 268 100 267 103 261 77 239 79 231 100 207 98 196 119 201 143 202 160 195 166 210 172 213 173 238 167 251 160 248 154 265 169 264 178 247 186 240 198 260 200 271 217 271 219 262 207 258 195 230 192 198 210 184 227 164 242 144 259 145 284 151 277 141 293 140 299 134 297 127 273 119 270 105
Polygon -7500403 true true -1 195 14 180 36 166 40 153 53 140 82 131 134 133 159 126 188 115 227 108 236 102 238 98 268 86 269 92 281 87 269 103 269 113

x
false
0
Polygon -7500403 true true 270 75 225 30 30 225 75 270
Polygon -7500403 true true 30 75 75 30 270 225 225 270
@#$#@#$#@
NetLogo 6.0.2
@#$#@#$#@
@#$#@#$#@
@#$#@#$#@
@#$#@#$#@
@#$#@#$#@
default
0.0
-0.2 0 0.0 1.0
0.0 1 1.0 0.0
0.2 0 0.0 1.0
link direction
true
0
Line -7500403 true 150 150 90 180
Line -7500403 true 150 150 210 180
@#$#@#$#@
0
@#$#@#$#@
