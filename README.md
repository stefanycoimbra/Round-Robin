# Round-Robin Scaling Algorithm Implementation

## <img src="https://img.icons8.com/color/100/000000/task--v1.png"/> What is Scaling?
Scaling is a way to administrate all the processes in the CPU. It controlls the change between the execution of the processes.

## Scheduling Criteria
* Burst Time: Quantity of the duration needed for a process.
* Turn Around Time: Interval that the process arrived and it was concluded.
* Waiting Time: Time that the process is in the ready state.
* Context Switching: store the state of a process so that it can be reloaded when required and execution can be resumed from the same point as earlier.
* Average Turn Around Time: is the average turn around time of the algorithm.
* Average Waiting Time: is the average waiting time of the algorithm.

## What is Round-Robin?
Round-Robin is a scheduling algorithm that consists of dividing the CPU usage time (Central Processing Unit). Each process receives a time slice, this time is called Quantum. The processes are all stored in circular Queue (Buffer).

## How can I run the code?
```
- First, you can download all the files, except by the .pdf;
- Then, you can compile the file RoundRobin.c;
- Next, you can rename the file saida.txt downloaded, see and compare the saida.txt generated with the other one;
- You can always modify the file dados.txt with your own data and the algorithm will generate the saida.txt;
```

## The input
The format of the input follow the pattern:

```
0 8 2
1 5 3
3 1 4
6 4 1
```

The fisrt column refers to the arrival time of each process, the second one the duration of each one and the last column rfers to the priority.

## The output
An example of an output:

```
Quantum: 3

* * * * * * * * * *
*     Legenda     *
*                 *
*  ## : Execucao  *
*   -- : Espera   *
*                 *
* * * * * * * * * *

Tempo	 P1 	P2 	P3 	P4 	
0-1 	 ## 	   	   	   	
1-2 	 ## 	-- 	   	   	
2-3 	 ## 	-- 	   	   	
3-4 	 -- 	## 	-- 	   	
4-5 	 -- 	## 	-- 	   	
5-6 	 -- 	## 	-- 	   	
6-7 	 -- 	-- 	## 	-- 	
7-8 	 ## 	-- 	   	-- 	
8-9 	 ## 	-- 	   	-- 	
9-10 	 ## 	-- 	   	-- 	
10-11 	 -- 	-- 	   	## 	
11-12 	 -- 	-- 	   	## 	
12-13 	 -- 	-- 	   	## 	
13-14 	 -- 	## 	   	-- 	
14-15 	 -- 	## 	   	-- 	
15-16 	 ## 	   	   	-- 	
16-17 	 ## 	   	   	-- 	
17-18 	    	   	   	## 	

ID do processo 		 Tempo de Duracao 		 Tempo de Vida 		 Tempo de Espera  
Processo [1] 				 8 				 17 				 9
Processo [2] 				 5 				 14 				 9
Processo [3] 				 1 				 4 				 3
Processo [4] 				 4 				 12 				 8      

Tempo medio de vida:	11.75
Tempo medio de espera:	7.25
```

The fisrt line refers to the Quantum used, then we have a legend of the execution diagram, the proper one and we also have a table with each process and their burst time, turn around time and waiting time. The average turn around time and average waiting time are on the bottom ("Tempo médio de vida" e "Tempo médio de espera").

## Developers
* Camila Motta Renó (<img src="https://img.icons8.com/ios-glyphs/30/000000/github.png"/> https://github.com/kcami)
* Stéfany Coura Coimbra (<img src="https://img.icons8.com/ios-glyphs/30/000000/github.png"/> https://github.com/stefanycoimbra)
* Ytalo Ysmaicon Gomes (<img src="https://img.icons8.com/ios-glyphs/30/000000/github.png"/> https://github.com/ysmaicon)
