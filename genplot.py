import os
import pandas as pd
import matplotlib.pyplot as plt

BASE_DIR = 'resultados'
METRICAS = ['FLOPS_DP', 'L2', 'MEM']
MARCADORES = ['newton', 'jacobiana', 'sistema_linear']
TEMPOS = {
    'newton': 'Tempo Total (ms)',
    'jacobiana': 'Tempo Jacobiana (ms)',
    'sistema_linear': 'Tempo SL (ms)'
}

def main():
    paths = getVersionPaths()
    plotTempos(paths)
    plotMetricas(paths)

def getVersionPaths():
    versions = {}
    for version in os.listdir(BASE_DIR):
        version_path = os.path.join(BASE_DIR, version)
        
        if os.path.isdir(version_path):
            items = os.listdir(version_path)
            
            # Checa se os dados já estão diretamente nesta pasta (Estrutura Antiga)
            if 'tempos.csv' in items:
                versions[version] = version_path
            else:
                # Se não, procura pela subpasta do commit (Estrutura Nova)
                subpastas = [d for d in items if os.path.isdir(os.path.join(version_path, d))]
                if subpastas:
                    versions[version] = os.path.join(version_path, subpastas[0])
                    
    return versions

def plotTempos(paths):
    tempos = {}
    for version, path in paths.items():
        file_path = os.path.join(path, 'tempos.csv')
        if os.path.exists(file_path):
            tempos[version] = pd.read_csv(file_path)
    for key, title in TEMPOS.items():
        plot(
            data_frames=tempos,
            title=title,
            ylabel='Tempo (ms)',
            x_col='Tamanho',
            y_col=TEMPOS[key],
            output_filename=f'tempos_{key}.pdf'
        )

def plotMetricas(paths):
    for metrica in METRICAS:
        for marcador in MARCADORES:
            values = {}
            filename = f'{marcador}_{metrica}.csv'           
            for version, path in paths.items():
                file_path = os.path.join(path, filename)
                
                # Só tenta ler se o arquivo existir
                if os.path.exists(file_path):
                    values[version] = pd.read_csv(file_path, header=None, usecols=[0, 1], names=['N', 'Valor']) 
            
            # Só tenta desenhar o gráfico se tiver encontrado dados
            if values:
                plot(
                    data_frames=values,
                    title=metrica,
                    ylabel=metrica,
                    x_col='N',
                    y_col='Valor',
                    output_filename=f'{marcador}_{metrica}.pdf'
                )

def plot(data_frames, title, ylabel, x_col, y_col, output_filename):
    fig, ax = plt.subplots()
    ax.set_title(title) 
    x_vals = set()

    for version, df in data_frames.items():
        ax.plot(df[x_col], df[y_col], 'o-', label=version)
        x_vals.update(df[x_col])

    ax.set(xlabel='Tamanho do SNLB (N)', ylabel=ylabel, xscale='log', yscale='log')
    
    if x_vals:
        sorted_x = sorted(x_vals)
        ax.set_xticks(sorted_x)
        ax.set_xticklabels(sorted_x, rotation=45)
        
    ax.grid(True, which="both", ls="--", alpha=0.5)
    ax.legend()

    plt.tight_layout()
    plt.savefig(f'{BASE_DIR}/{output_filename}')
    plt.close()

if __name__ == '__main__':
    main()