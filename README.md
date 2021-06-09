# continuous_integration

This is sample app presenting simulation of CI env (Jenkins, Bamboo). 

## Core concepts:
- every `Repository` has it's own thread running `webhook()` method which generates `Task` instances
- every `Agent` is a resource with random amount of CPU/MEM.
- `Environment` is a resource.
- `Dispatcher` handles critical sections on Agents and Environment.
- Console window is separate thread from method `writeState()`
- Keyboard input is handled by thread based on `listenForKey()`

## Lifecycle:
i. Repository is in idle state for random period.

ii. Build phase:
  1. Repository creates new Task with random quota on CPU/MEM and random duration.
  2. Repository asks Dispatcher to allocate slot on one of the Agents that have sufficent resources for that Task.
  3. Task runs for defined duration.
  4. Resources on agent are released. 
  
iii. Deploy phase:
  1. Repository creates new Task with random quota on CPU/MEM and random duration.
  2. Repository asks Dispatcher to allocate slot on one of the Agents that have sufficent resources for that Task.
  3 Repository asks for lock on Environment.
  4. Task runs for defined duration.
  5. Resources on agent and lock on environment are released.
  
iv. Back to 1.
