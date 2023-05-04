#ifndef SR_INTERFACE_H_
#define SR_INTERFACE_H_

void  SR_init       (void);
void  SR_updateTask (struct pt* pt, uint32_t interval);

#endif