import sys
import numpy as np

def rebuild_data(thebytes):
    event_data=np.frombuffer(thebytes, dtype=np.uint8, count=1928)
    
    event_data_list=event_data.tolist()
    
    frame_list=event_data_list
    
    #delete  event_header and event_footer
    for j in range(0, 4):
        frame_list.remove(frame_list[0])
        frame_list.pop()
    
    mark=0
    tmp_list=np.zeros(1536, dtype=int)
    x=0
    
    for k in range(0, 1920):
        if k%40==0 or  k%40==1 or k%40==2 or k%40==3 or k%40==36 or k%40==37 or k%40==38 or k%40==39:
            x=1
        if x==0:
            tmp_list[mark]=frame_list[k]
            mark=mark+1
        x=0
        
    frame_list=tmp_list
    #length=len(frame_list)
    #print(length)
    
    frame_array=np.reshape(frame_list, newshape=(48, 16, 2))
    
    frame=np.zeros((48, 16), dtype=int)
    for i in range(0, 48):
        for j in range(0, 16):
            tmp=frame_array[i, j, 0]*256+frame_array[i, j, 1]
            frame[i, j]=tmp
    return frame

if __name__ == "__main__":
    sys.exit()        
