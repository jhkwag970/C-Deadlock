<h1>Deadlock Detection and Recovery</h1>

In they OS, after process are assgined the resources, they are not volantilry give up the resources until they can finish the processs.And deadlock is occured whene there are cycle among the process. Specificly, no process cannot finish their work with the avaiable resources in the system. Thus, using the deadlock detection algorithm, user can decide whether there is a deadlock among process. Then, if there is deadlock, use the recovery alogirthm to resolve the cycle.  

<h2>Author by Jiyong Kwag</h2>

<h2>Purpose</h2>

There are a number of solution for deadlock: deadlock avoidance, deadlock prevention, ignore deadlock, and lastly deadlock detection and recovery. Genrally, ignore the deadlock is the known to be the most sufficient way to solve the deadlock and deadlock detection and recovery is a least usefull way. However, ignore deadlock is de-allocating the resource that is assigned to the process to solve the deadlock and re-run. This method is known to be good because there are low likely that deadlock will occur in the system. But, de-allocate the resource from process would cause user to lose the information that they already had.

 Deadlcok detection and recovery is the least useful way because system alwasy has to check whether there is deadlock and run the recovery algorithms which can cause the overhaed in the system. However, it can be useful because, system does not have to de-allocate all the resources that each process has been assigned. In this lab4, I am trying to find the more sufficient way to discover the recoery alogrithm that can be solved without calling algorithm too much. Thus, OS can have more efficient choices to solve the deadlock in the system


Date: 2022-12-5

