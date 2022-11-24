# Structure of the data files

## File structure

1. The first folder of the data tree reflects the name of the test object
2. Each separate experimental series has been stored in a folder whose name is the date
3. Each folder contain data file from the crio measurement system 'data_crio' and:
   1. Optional folder with measurements from the IMU system 'data_shimmer'
   2. Optional 'movies' folder

## How to interpret file names

* h : height of the dumped load in millimetres
* m : weight of the dumped load in kilograms with optional "+" tag meaning extra weight  
* vc : valve open with optional open ratio
* vc : valve closed with optional close ratio
* last digit : test number

1. ex: h3500_m5_vo_2 ->
   1. height 3500 mm
   2. weigth 5 kg
   3. valves open
   4. test 2