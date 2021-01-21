set -u

gnome-terminal -- bash -c "client/bin/a.out client/config/RootServers client/config/RequeteFile; bash"
gnome-terminal -- bash -c "server/bin/a.out server/config/RootServer 4430 4445  1;  bash"
gnome-terminal -- bash -c "server/bin/a.out server/config/RootServer 4443 4400  5; bash"
gnome-terminal -- bash -c "server/bin/a.out server/config/SubDomainServer 4000 4449 1;bash"
gnome-terminal -- bash -c "server/bin/a.out server/config/SubDomainServer 4022 4448 1; bash"
gnome-terminal -- bash -c "server/bin/a.out server/config/MachineNameServer 4451 4450 2; bash"

set -x
