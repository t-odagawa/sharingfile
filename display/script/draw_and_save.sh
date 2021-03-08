cd ${EVENT_DISPLAYDIR}/bin

inputdir=${HOME}/data/hitconv
inputfile=${inputdir}/neutrino_b2physics_fullsetup_fullstat_timedifcut0_loose_ninja_$1_$2_$3.root

outputdir=${HOME}/SharingFile/display/output

geometrydir=${HOME}/Programs/WagasciMC/etc/wagasci/b2/geometry

event=$4

command=${HOME}/SharingFile/display/input/command.txt

./EventDisplay --input-file ${inputfile} --output-directory ${outputdir} --geometry-directory ${geometrydir} --start-event ${event} < ${command}
