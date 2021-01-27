/* 
 * File:   vmCircularQ.h
 * Author: AmitDP
 *
 * Created on February 26, 2020, 2:17 PM
 */

#ifndef VM_CIRCULAR_Q_H
#define	VM_CIRCULAR_Q_H


#define SIZE_RCV_QUEUE 200

typedef struct _ST_RCV_QUEUE
{
    int8 m_ui8QArr[SIZE_RCV_QUEUE];
    int8 m_ui8Front;
    int8 m_ui8Rear;
} ST_RCV_QUEUE;

ST_RCV_QUEUE g_stRcvQueue;
unsigned int8 g_ui8ElementInQ = 0;

void InitializeRcvQ()
{
    memset(g_stRcvQueue.m_ui8QArr, SIZE_RCV_QUEUE, 0);
    g_stRcvQueue.m_ui8Front = -1;
    g_stRcvQueue.m_ui8Rear  = -1;
}

//int isFull()
//{
//    if( (front == rear + 1) || (front == 0 && rear == SIZE_RCV_QUEUE-1)) return 1;
//    return 0;
//}
//
//int8 items[SIZE_RCV_QUEUE];
//int front = -1, rear =-1;
unsigned int8 ElementsInQ(void)
{
    unsigned int8 ui8Retval = 10;
    
    if (-1 == g_stRcvQueue.m_ui8Front)
    {
        //return = 0;
        ui8Retval = 0;
        return ui8Retval;
    }
    
    
    if (g_stRcvQueue.m_ui8Front > g_stRcvQueue.m_ui8Rear)
    {
        //return = (SIZE_RCV_QUEUE - g_stRcvQueue.m_ui8Front + g_stRcvQueue.m_ui8Rear + 1);
        ui8Retval = (SIZE_RCV_QUEUE - g_stRcvQueue.m_ui8Front + g_stRcvQueue.m_ui8Rear + 1);
    }
    else
    {
        //return = (g_stRcvQueue.m_ui8Rear - g_stRcvQueue.m_ui8Front + 1);
        ui8Retval = (g_stRcvQueue.m_ui8Rear - g_stRcvQueue.m_ui8Front + 1);
    }
    
    return ui8Retval;
}

unsigned int8 IsQFull(void)
{
    unsigned int8 ui8RetVal = 10;
    
    if( (g_stRcvQueue.m_ui8Front == g_stRcvQueue.m_ui8Rear + 1) ||
        (g_stRcvQueue.m_ui8Front == 0 && g_stRcvQueue.m_ui8Rear == SIZE_RCV_QUEUE - 1))
    {
        // Queue is Full
        ui8RetVal = 1;
    }
    else
    {
        ui8RetVal = 0;
    }   
    return ui8RetVal;
}

//int isEmpty()
//{
//    if(front == -1) return 1;
//    return 0;
//}

unsigned int8 IsQEmpty(void)
{
    unsigned int8 ui8RetVal = 10;
    
    if(g_stRcvQueue.m_ui8Front == -1)
    {
        ui8RetVal = 1;
    }
    else
    {
        ui8RetVal = 0;
    }
    return ui8RetVal;
}

//void enQueue(int element)
//{
//    if(isFull()) printf("\n Queue is full!! \n");
//    else
//    {
//        if(front == -1) front = 0;
//        rear = (rear + 1) % SIZE_RCV_QUEUE;
//        items[rear] = element;
//        printf("\n Inserted -> %d", element);
//    }
//}

void EnQueue(unsigned int8 ui8Element)
{
    if (-1 == g_stRcvQueue.m_ui8Front)
    {
        g_stRcvQueue.m_ui8Front = 0;
    }
    g_stRcvQueue.m_ui8Rear = (g_stRcvQueue.m_ui8Rear + 1) % SIZE_RCV_QUEUE;
    
    g_stRcvQueue.m_ui8QArr[g_stRcvQueue.m_ui8Rear] = ui8Element;
}

unsigned int8 DeQueue(void)
{
    unsigned int8 ui8Element = 0;
    unsigned int8 ui8RetVal  = 0;
    
    ui8RetVal = IsQEmpty();
    if(ui8RetVal)
    {
        ui8RetVal = 0;
        //return 0; //??
    }
    else
    {
        ui8Element = g_stRcvQueue.m_ui8QArr[g_stRcvQueue.m_ui8Front];
        if (g_stRcvQueue.m_ui8Front == g_stRcvQueue.m_ui8Rear)
        {
            g_stRcvQueue.m_ui8Front = -1;
            g_stRcvQueue.m_ui8Rear  = -1;
        } /* Q has only one element, so we reset the queue after dequeing it. ? */
        else
        {
            g_stRcvQueue.m_ui8Front = (g_stRcvQueue.m_ui8Front + 1) % SIZE_RCV_QUEUE;
        }
        //printf("\n Deleted element -> %d \n", ui8Element);
        ui8RetVal = ui8Element;
    }
    
    return ui8RetVal;
}

//int deQueue()
//{
//    int element;
//    if(isEmpty()) {
//        printf("\n Queue is empty !! \n");
//        return(-1);
//    } else {
//        element = items[front];
//        if (front == rear){
//            front = -1;
//            rear = -1;
//        } /* Q has only one element, so we reset the queue after dequeing it. ? */
//        else {
//            front = (front + 1) % SIZE_RCV_QUEUE;
//            
//        }
//        printf("\n Deleted element -> %d \n", element);
//        return(element);
//    }
//}


#endif	/* VM_CIRCULAR_Q_H */

