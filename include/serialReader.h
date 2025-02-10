#define CREATE_TASK(task_name, stack_size, priority) xTaskCreate(task_name, #task_name, stack_size, NULL, priority, &task_name##Handle)
#define STACK_SIZE(x) (x) 
#define PRIORITY(x) (x)

void SerialReaderTask(void *parameter);