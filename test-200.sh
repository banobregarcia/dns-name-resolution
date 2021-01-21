#Verification mémoire

SEED=`od -An -N2 -i /dev/urandom`
BET=$((`od -An -N1 -i /dev/urandom` % 37))

valgrind --leak-check=full --trace-children=yes --error-exitcode=100 $PROG $SEED $BET > /dev/null 2>&1
test $? = 100 && echo "** Echec du test : pb mémoire" && return 1
