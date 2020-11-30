%поиск путей в графе (DFS)
door(a,b).
door(b,c).
door(b,d).
door(c,d).
door(d,e).
door(d,f).
door(f,g).
door(h,i).

member1(X,[X|_]).
member1(X,[_|T]):-member1(X,T).

move(X,Y):-door(X,Y);door(Y,X).

path(X,Y,P):-path1([X],Y,P).

prolong([X|T],[Y,X|T]):-move(X,Y), not(member1(Y,[X|T])).

path1([X|T],X,[X|T]).
path1(P,Y,R):-
    prolong(P,P1),
    path1(P1,Y,R).
