clear all;
clc;
delete(instrfind);

BUFF_SIZE=250;

s=serial('COM14', 'BaudRate', 115200,'Terminator','LF','TimeOut',20);
fopen(s);

data=[];
t=[];
try
    for i=1:1:BUFF_SIZE
        data(i,:) = fscanf(s,'%f%c%c%f')';

        if(mod(i,50)==0)
            disp(['adquired ' num2str(i) ' samples']);
        end
    end
    t=data(:,1)/1e3;%time sent in milliseconds and converted to seconds
    t(t==0)=NaN;
    circ_buffer1=data(:,2);
    circ_buffer2=data(:,3);
    debug_data1=data(:,4);
    fclose(s);

catch Me
    Me.identifier
    close all;
    disp("Error. Closing serial port...");
    fclose(s);
    return;
end


% Ready	eReady
% Running	eRunning (the calling task is querying its own priority)
% Blocked	eBlocked
% Suspended	eSuspended
% Deleted	eDeleted (the tasks TCB is waiting to be cleaned up)
% /* Task states returned by eTaskGetState. */
% typedef enum
% {
%     eRunning = 0,   /* A task is querying the state of itself, so must be running. */
%     eReady,         /* The task being queried is in a read or pending ready list. */
%     eBlocked,       /* The task being queried is in the Blocked state. */
%     eSuspended,     /* The task being queried is in the Suspended state, or is in the Blocked state with an infinite time out. */
%     eDeleted,       /* The task being queried has been deleted, but its TCB has not yet been freed. */
%     eInvalid        /* Used as an 'invalid state' value. */
% } eTaskState;

eRunning=0;
eReady=1;
eBlocked=2;
eSuspended=3;
eDeleted=4;
eInvalid=5;

circ_buffer1=subs(circ_buffer1,{eRunning,eReady,eBlocked,eSuspended},[2 1 0 2.5]);
circ_buffer2=subs(circ_buffer2,{eRunning,eReady,eBlocked,eSuspended},[2 1 0 2.5]);

close all;
figure;
hold on;
grid on;
box on;
stairs(t,circ_buffer1+0);
stairs(t,circ_buffer2+5);

% set(gca,'XTick',[0:0.100:max(t)]);
set(gca,'YTick',[0 1 2 2.5  5 6 7 7.5  10 11 12 12.5  15 16 17 17.5  20 21 22 22.5  25 26 27 27.5]);
set(gca,'YTickLabel',{'Ready','Blocked','Running','Suspended','Ready','Blocked','Running','Suspended','Ready','Blocked','Running','Suspended','Ready','Blocked','Running','Suspended','Ready','Blocked','Running','Suspended','Ready','Blocked','Running','Suspended'});
set(gca,'XMinorTick','on');
% ylim([-1 19]);

% figure;
% hold on;
% grid on;
% box on;
% plot(t,debug_data1);
% legend('data');
