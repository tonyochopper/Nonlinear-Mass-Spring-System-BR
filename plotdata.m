figure(1);
axis([0 10 0 10])
for i=1:1
%s = num2str(i*0.001);
s ="0.000x"
s2 ="0.000y"
if(length(s)==4)
    s =strcat(s, "0");
end

if(length(s)==3)
    s = strcat(s, "00");
end
s = strcat(s, ".csv");
disp(s)
test = csvread("0.000X.csv");
test2 = csvread("0.000Y.csv");
x = test;
y = test2;
z =  sin(i*0.001);


%x = [5,3,1];
%y = [1,4,2];
%plot(x,y,'-o','MarkerSize',10, 'MarkerEdgeColor','red');
%plot(x,y,'Layout','force')

node1 = [1, 1]
node2 = [2, 3]

%x=[x_pos(nodea), x_pos(nodeb), x_pos(nodea), x_pos(nodeb)]
%y=[y_pos(nodea), y_pos(nodeb), y_pos(nodea)]
    

s = csvread("s.csv")
t = csvread("t.csv")
G = graph(s,t);
G =addnode(G,1);
%x = [test(1,1) test(1,3) test(2,1)];
%y = [test(1,2) test(1,4) test(2,2)];



%test(:,1)
plot(G,'XData',x,'YData',y);


drawnow;
end