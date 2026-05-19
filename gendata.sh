#!/bin/bash
export PATH="/home/soft/likwid/bin:/home/soft/likwid/sbin:${PATH}"

CMD_DIR=$(dirname $0)
RESULTS_DIR=${CMD_DIR}/resultados
OUT_DIR=${CMD_DIR}/out

PROG=${1:-broyden}
tipo=${2:-avx}
CPU=${3:-3}
GRUPOS="FLOPS_DP L2 MEM"
TAMANHOS="32 64 128 256 512 1000 2000 4000"
#TAMANHOS="6000 7000"
#TAMANHOS="10000"
#TAMANHOS="50000"
#TAMANHOS="100000"
LIKWID_LOG="likwid.log"

# Valor perto da raiz para garantir que converge
X0=-1.0

# Definidos pelo enunciado
EPSILON=0
MAX_IT=25

BRANCH="HEAD"
GIT_HASH=$(git rev-parse --short ${BRANCH})

make clean && make
echo "performance" >/sys/devices/system/cpu/cpufreq/policy${CPU}/scaling_governor

mkdir -p ${RESULTS_DIR}/${GIT_HASH}
mkdir -p ${OUT_DIR}
LIKWID_LOG="${RESULTS_DIR}/${GIT_HASH}/${LIKWID_LOG}"
CSV_OUT="${RESULTS_DIR}/${GIT_HASH}/tempos.csv"
echo "Tamanho,Tempo Total (ms),Tempo Jacobiana (ms),Tempo SL (ms)" > "${CSV_OUT}"

PRIMEIRO_GRUPO=$(echo $GRUPOS | awk '{print $1}')

for grupo in $GRUPOS; do
  rm -f ${LIKWID_LOG}
  for n in $TAMANHOS; do
    LIKWID_OUT="${OUT_DIR}/likwid.txt"
    PROG_OUT="${OUT_DIR}/prog_out.txt"
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
  mv ${RESULTS_DIR}/*.csv ${RESULTS_DIR}/${GIT_HASH}/ 2>/dev/null
done

make clean
rm -rf ${OUT_DIR} 2>/dev/null

echo "powersave" >/sys/devices/system/cpu/cpufreq/policy${CPU}/scaling_governor
