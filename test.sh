#
# Fonctions et variables auxiliaires utilisées pour les différents
# tests.
#

TEST=$(basename $0 .sh)

TMP=/tmp/$TEST-$$
LOG=$TEST.log

DEBUG_PORT=${DEBUG_PORT:-1} ; export DEBUG_PORT

# Rediriger stderr vers le log pour voir les résultats des tests
# On conserve stdout inchangé, il faudra le rediriger à la demande
exec 2> $LOG

set -u			# erreur si utilisation d'une variable non définie

MARGE=0.02		# 20 ms devraient suffire pour avoir une marge

###############################################################################
# Fichiers de log auxiliaires

logs_aux ()
{
    set +x
    (
	rm -f $CHRONO	# pour ne pas perturber l'affichage des logs
	for f in $(ls -d /tmp/* | grep "^$TMP")
	do
	    echo "==> log aux $f"
	    cat $f
	done
	rm -f $TMP.*
	if [ "$(uname)" = Linux ]
	then
	    echo "==> /dev/shm"
	    ls /dev/shm
	fi
    ) >&2
}

###############################################################################
# Une fonction qu'il vaudrait mieux ne pas avoir à appeler...

fail ()
{
    echo "==> Échec du test '$TEST' sur '$1'."
    echo "==> Log : '$LOG'."
    logs_aux
    echo "==> Exit"
    exit 1
}

###############################################################################
# Teste la présence du traditionnel message : "usage: prog arg..." dans $TMP
# Renvoie vrai si trouvé, faux si pas trouvé

tu ()
{
    # rappel: "! cmd" => inverse le code de retour de cmd
    ! grep -q "usage: " $TMP
}



set -u

PROG_C = "client/bin/a.out"
PROG_S = "server/bin/a.out"

# Vérifie les arguments passés au programme
test1 () {
	if ! $PROG_C             2>&1 | grep -i "Usage" > /dev/null; then echo "** Echec du test: appel sans argument"    && return 1; fi
	if ! $PROG_C 1           2>&1 | grep -i "Usage" > /dev/null; then echo "** Echec du test: appel avec 1 argument"  && return 1; fi
	if ! $PROG_C 1 2 3 4     2>&1 | grep -i "Usage" > /dev/null; then echo "** Echec du test: appel avec 4 arguments" && return 1; fi

	if ! $PROG_S             2>&1 | grep -i "Usage" > /dev/null; then echo "** Echec du test: appel sans argument"    && return 1; fi
	if ! $PROG_S 1           2>&1 | grep -i "Usage" > /dev/null; then echo "** Echec du test: appel avec 1 argument"  && return 1; fi
	if ! $PROG_S 1 2 3       2>&1 | grep -i "Usage" > /dev/null; then echo "** Echec du test: appel avec 3 arguments" && return 1; fi
	if ! $PROG_S 1 2 3 4 5   2>&1 | grep -i "Usage" > /dev/null; then echo "** Echec du test: appel avec 5 arguments" && return 1; fi

	return 0
}

# Vérification mémoire
test2 () {
	SEED=`od -An -N2 -i /dev/urandom`
	BET=$((`od -An -N1 -i /dev/urandom` % 37))

	valgrind --leak-check=full --trace-children=yes --error-exitcode=100 $PROG $SEED $BET > /dev/null 2>&1
	test $? = 100 && echo "** Echec du test : pb mémoire" && return 1

	return 0
}

#Vérification existence et lisibilité des arguments de client
test3 () {
FILE1 = "client/config/RootServers"
FILE2 = "client/config/RequeteFile"
if [ -r $FILE1 ] && [ -r $FILE2 ]; then
   echo "The files '$FILE1' '$FILE2' exist and they are readable."
else
   echo "The files '$FILE1' '$FILE2' are not found."
fi

}


#Vérification existence et lisibilité des arguments de serveur
test4 () {
FILE3 = "server/config/RootServer"
FILE4 = "server/config/SubDomainServer"
FILE5 = "server/config/MachineNameServer"
if [ -r $FILE3 ] && [ -r $FILE4 ] && [ -r $FILE5 ]; then
   echo "The files '$FILE3' '$FILE4' '$FILE5'exist and they are readable."
else
   echo "The files '$FILE3' '$FILE4' '$FILE5' are not found."
fi

}




#Test avec 5 serveurs et un client
test5 () {
set -u

gnome-terminal -- bash -c "client/bin/a.out client/config/RootServers client/config/RequeteFile; bash"
gnome-terminal -- bash -c "server/bin/a.out server/config/RootServer 4430 4445  1;  bash"
gnome-terminal -- bash -c "server/bin/a.out server/config/RootServer 4443 4400  5; bash"
gnome-terminal -- bash -c "server/bin/a.out server/config/SubDomainServer 4000 4449 1;bash"
gnome-terminal -- bash -c "server/bin/a.out server/config/SubDomainServer 4022 4448 1; bash"
gnome-terminal -- bash -c "server/bin/a.out server/config/MachineNameServer 4451 4450 2; bash"

set -x
}

# Lance les tests
for T in $(seq 1 5)
do
	echo "== Test $T ..."
	if test$T; then
		echo "== Test $T : ok $T/5"
	else
		echo "== Test $T =="

		if [ "$T" -eq 1 ]; then
			echo "   Rappel, en cas d'un nombre d'argument invalide vous devez afficher :"
			echo "      Usage: ./a.out ..."
		else
			return 1
		fi
		if [ "$T" -eq 5 ]; then
			echo "	Rappel, ce test lance les terminaux pour pouvoir tester les performances, la charge et autres "
		fi
	fi
done

set -x
