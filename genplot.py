#! /usr/bin/env python3

from string import *
from math import *
import os, re, sys, csv

campos = { 
    "L2": [("L2", r"L3 miss ratio")],
    "FLOPS_DP": [("FLOPS_DP", r"DP \[*MFLOP/s\]*"), ("FLOPS_AVX", r"AVX DP \[*MFLOP/s\]*")],
    "MEM": [("MEM", r"Memory bandwidth \[*MB/s\]*")],
}

def lerDados():
    dados = {}
    linhas = list(sys.stdin)
    marker = None
    tamanho = None
    grupo = None
    
    for linha in linhas:
        if "TABLE,Region" in linha and "Metric," in linha:
            partes = linha.split(',')
            grupo = partes[3]
            marker, tamanho = re.sub(r"Region ", "", partes[1]).rsplit('_', 1)
        elif grupo:
            padroes = campos.get(grupo)
            for metrica, padrao in padroes:
                if padrao and re.match(padrao, linha) is not None:
                    valor = float(linha.split(',')[1])
                    if marker not in dados:
                        dados[marker] = {}
                    if grupo not in dados[marker]:
                        dados[marker][grupo] = {}
                    if tamanho not in dados[marker][grupo]:
                        dados[marker][grupo][tamanho] = {}
                    dados[marker][grupo][tamanho][metrica] = valor
    return dados

def formatarDados(dados):
    for marker in dados:
        for grupo in dados[marker]:
            for tamanho in dados[marker][grupo]:
                if grupo == "CLOCK":
                   for metrica, valor in dados[marker][grupo][tamanho].items():
                       if metrica == "TIME":
                           if valor is not None:
                               dados[marker][grupo][tamanho][metrica] = valor * 1000
    return dados

def salvarResultados(dados):
    os.makedirs("resultados", exist_ok=True)
    for marker in dados:
        for grupo in dados[marker]:
            metricas = [metrica for metrica, _ in campos.get(grupo, [])]
            plotFile = os.path.join("resultados", f'{marker}_{grupo}.csv')
            with open(plotFile, "a") as fp:
                writer = csv.writer(fp)
                if not os.path.exists(plotFile):
                    writer.writerow(['tamanho'] + metricas)
                for tamanho in sorted(dados[marker][grupo], key=lambda x: int(x)):
                    valores = [tamanho] + [str(dados[marker][grupo][tamanho].get(metrica, '')) for metrica in metricas]
                    writer.writerow(valores)


def main():
    dados = lerDados()
    dados = formatarDados(dados)
    salvarResultados(dados)

if __name__ == "__main__":
    main()
