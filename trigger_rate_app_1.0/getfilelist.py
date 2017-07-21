#! /home/nsanthony/anaconda3/bin/python
import os
from time import gmtime, strftime
from datetime import date, timedelta, datetime


def getfilelist(start=None,end=None,path=None,last=None,move_back=None):
    multi_day = 0
    if move_back == None:
        move_back = 0
    #last=1 means using last file, last=0 means not using just last file
    if start == None:
        if move_back == 0: #this looks to see if this is a recursive call
            start = strftime('%Y%m%d-000000',gmtime())
        elif move_back > 0:
            #this rolls the date back by one so that it can try and find a file
            #if none was found before now
            date_back = datetime.utcnow().date() - timedelta(days=move_back)
            start = date_back.strftime("%Y%m%d-000000")
        if last == None:
            last = 1
        
        
    year = start[0:4]
    month = start[4:6]
    day = start[6:8]
    hour_start = start[9:11]
    minute_start = start[11:13]

#this sets the end date to be the start date since none was given
    if end != None:
        multi_day = 1
        day_end = end[6:8]
        hour_end = end[9:11]
        minute_end = end[11:13]
    elif end == None:
        day_end = day
        hour_end = '23'
        minute_end = '59'
#checks to see if the call included a last file command or not
#last = 1 means only return last file, 0 is return all but last 
#(since it may be incomplete)        
    if last == None:
        last = 0
    elif last != None:
        if last != 0:
            last = 1
        
    if path == None:
        L0_directory= '/home/nsanthony/cream_lab/L0data'
    
    #This is a bash file that finds all the files in the desired date
    os.chdir(L0_directory)
    os.system('./makelist %s %s %s 1'%(year,month,day))
    
    #This adds any extra days to the file list
    if multi_day == 1:
        if day != day_end:
            d_s = int(day)
            d_e = int(day_end)
            for i in range((d_s+1),(d_e+1)):
                os.system('./makelist %s %s %s 0'%(year,month,i))
    #opens the LIST file that was just created
    cream_dir = L0_directory + '/cream'
    os.chdir(cream_dir)
    file_list = open('LIST','r')
    f = file_list.readlines()
    file_list.close()
    #This make sure the first file is the first one after the start time
    done = 0
    loop_num = 0
    if len(f) != 1 & len(f) != 0:
        while done == 0:
            part = f[loop_num].partition('%s/'%day)
            first_file_time = int(part[2][len(part[2])-11:len(part[2])-7])
            target_time = int(hour_start + minute_start)
            if first_file_time < target_time:
                loop_num += 1
            else:
                done = 1           
        f = f[loop_num:len(f)]
        
    #this makes sure that the last file is the last file before end time    
    number_of_files = len(f)
    keep_number = number_of_files
    done = 0
    if number_of_files != 1 & len(f) != 0:
        while done == 0:
            part = f[keep_number-1].partition('%s/'%day_end)
            filenamelen = len(part[2])
            last_file_time = int(part[2][filenamelen-11:filenamelen-7])
            target_time = int(hour_end + minute_end)
            if last_file_time > target_time:
                keep_number -= 1
            else:
                done = 1
        f = f[0:keep_number]
    number_of_files = len(f)

           
        
        
    if last == 1:
        if len(f) == 0:
            #recursive algolrithim to search for most recent COMPLETE file if none was found
            move_back += 1
            q = getfilelist(last=1,move_back=move_back)
            return q
        else:
            #if last file found it returns it
            q = f[keep_number - 2]
            send = [q]
            return send
    else:
        #returns all but most recent file (since this one may not be complete)
        return f[0:number_of_files - 1]



