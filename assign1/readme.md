# Assignment 1
## Team Members

| std_id   | Name             |
| -------- | ---------------- |
| k21-3080 | Mustafa Hashmani |
| k21-4710 | Abdullah Baqai   |
| k21-3007 | Areeb Nadeem     |

## Output Screenshots
**Shell Script**
![image](https://github.com/NUCES-Khi/matrixtimesvector-team-deadline-extend/assets/63756923/dd3d723a-7090-4ef9-a170-948a5dd8676f)
**CSV FILE**
![image](https://github.com/NUCES-Khi/matrixtimesvector-team-deadline-extend/assets/63756923/8321f3b2-cd4d-4171-a147-da0ddb071aef)
**GRAPH**
![image](https://github.com/NUCES-Khi/matrixtimesvector-team-deadline-extend/assets/63756923/736f25fb-aaf6-4f7f-b44d-5f1069e32f74)




## Results and Analysis
* Both OMP_Naive and OMP_tiling yield identical outcomes.
* MPI_tiling exhibited the most optimal performance, showcasing the shortest execution time across all matrix sizes.
* Contrarily, MPI_Naive fared even poorer than the sequential approach.
* For smaller matrix sizes, the parallel programs fail to deliver any noticeable speedup.

## Major Problems Encountered
1 - Syntax Issues: One of the primary hurdles encountered was the emergence of syntax problems when trying to execute the shell script. These issues surfaced as "operand expected" or "syntax error" messages during script runtime. Despite double-checking the syntax, these obstacles persisted, creating uncertainty regarding their underlying cause.
**Resolved**

2 - Troubleshooting MPI and OpenMP Invocation: Addressing the invocation of MPI and OpenMP programs within the shell script proved to be a challenge. Ensuring the accurate specification of command-line parameters and the appropriate execution environment for these parallel applications necessitated meticulous attention to detail. Inaccurate invocation led to unexpected behavior and inaccurate outcomes.
**Resolved**

3 - Ensuring Uniform Execution: Attaining consistent execution of MPI and OpenMP programs across multiple iterations presented a formidable task. Fluctuations in execution times attributable to system load, resource contention, or other environmental variables posed challenges in obtaining dependable performance metrics. Guaranteeing that each program ran under comparable conditions for precise comparison demanded careful deliberation.
**Resolved**
