#!/bin/bash
export PATH="/home/soft/likwid/bin:/home/soft/likwid/sbin:${PATH}"

CMD_DIR=$(dirname $0)

PROG=${1:-matmult}
tipo=${2:-avx}
CPU=${3:-3}
GRUPOS="FLOPS_DP L2 MEM"
TAMANHOS="32 64 128 256 512 1000 2000 4000"
#TAMANHOS="6000 7000"
#TAMANHOS="10000"
#TAMANHOS="50000"
#TAMANHOS="100000"
LIKWID_LOG="likwid.log"
X0=-1.0
EPSILON=0
MAX_IT=25

BRANCH="HEAD"
GIT_HASH=$(git rev-parse --short ${BRANCH})

make clean && make
echo "performance" >/sys/devices/system/cpu/cpufreq/policy${CPU}/scaling_governor

mkdir -p ${CMD_DIR}/resultados/${GIT_HASH}
mkdir -p ${CMD_DIR}/out
LIKWID_LOG="${CMD_DIR}/resultados/${GIT_HASH}/${LIKWID_LOG}"
CSV_OUT="${CMD_DIR}/resultados/${GIT_HASH}/tempos.csv"
echo "Tamanho,Tempo Total (ms),Tempo Jacobiana (ms),Tempo SL (ms)" > "${CSV_OUT}"

PRIMEIRO_GRUPO=$(echo $GRUPOS | awk '{print $1}')

for grupo in $GRUPOS; do
  rm -f ${LIKWID_LOG}
  for n in $TAMANHOS; do
    LIKWID_OUT="${CMD_DIR}/out/likwid.txt"
    PROG_OUT="${CMD_DIR}/out/prog_out.txt"
    echo "$n $X0 $EPSILON $MAX_IT" | likwid-perfctr -O -C ${CPU} -g ${grupo} -o ${LIKWID_OUT} -m ./${PROG} > ${PROG_OUT}

    if [ "$grupo" == "$PRIMEIRO_GRUPO" ]; then
        T_TOTAL=$(grep "# Tempo Total:" ${PROG_OUT} | awk '{print $4}')
        T_JAC=$(grep "# Tempo Jacobiana:" ${PROG_OUT} | awk '{print $4}')
        T_SL=$(grep "# Tempo SL:" ${PROG_OUT} | awk '{print $4}')
        
    
        echo "$n,$T_TOTAL,$T_JAC,$T_SL" >> "${CSV_OUT}"
    fi

    cat ${LIKWID_OUT} >>${LIKWID_LOG}
    rm -f ${LIKWID_OUT} ${PROG_OUT}
  done
  ${CMD_DIR}/genplot.py <${LIKWID_LOG}
  mv *.csv ${CMD_DIR}/resultados/${GIT_HASH}/ 2>/dev/null
done

make clean
echo "powersave" >/sys/devices/system/cpu/cpufreq/policy${CPU}/scaling_governor
