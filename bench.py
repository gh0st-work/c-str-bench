import subprocess
from statistics import fmean, median

cached_bins: dict[tuple[str, str, str, tuple[str, ...]], None] = {}
def compile_and_run(compiler: str, source_file: str, output_file: str, *args: str) -> str:
    key = (compiler, source_file, output_file, args)
    if key not in cached_bins.keys():
        cmd = f'{compiler} {source_file} -o {output_file}'
        for arg in args:
            cmd += ' ' + arg
        compile_res = subprocess.run([cmd], stdout=subprocess.PIPE, shell=True)
        compile_res.check_returncode()
        cached_bins[key] = None
    res = subprocess.run([f'./{output_file}'], stdout=subprocess.PIPE, shell=True)
    res.check_returncode()
    return res.stdout.decode('utf-8')

def stats(l: list[float]) -> dict[str, float]:
    res = {'avg': fmean(l), 'med': median(l), 'min': min(l), 'max': max(l)}
    ls = sorted(l)
    ls_len = len(ls)
    for p in [5, 10, 90, 95]:
        res[f'p{p}'] = ls[max(1, round(p/100*ls_len))-1]
    return res

def stats_print(stats: dict[str, float], is_percent: bool = True, label: str = '') -> None:
    print(f'{label} ', end='')
    first_printed = False
    for k, v in stats.items():
        if first_printed:
            print('| ', end='')
        if is_percent:
            print(f'{k}: {v:+8.2f}% ', end='')
        else:
            print(f'{k}: {v: 9.6f} ', end='')
        first_printed = True
    print()

def main():
    runs = 10000
    progress_parts: None|int = 10
    draw_treshold_pc = 7
    for compiler in ['clang', 'gcc']:
        for optimization in ['3', 'fast']:
            data: dict[str, list[float]] = {}
            for run_i in range(runs):
                if progress_parts is not None and run_i%(runs//progress_parts) == 0:
                    print(f'Running `{compiler} -O{optimization}` {run_i}-{min(runs, run_i+runs//progress_parts)-1} ...')
                stdout_str = compile_and_run(compiler, 'bench.c', 'bench', f'-O{optimization}')
                for line in stdout_str.strip().splitlines():
                    info, dt_str = line.split(': ')
                    key, value = info.strip(), float(dt_str.strip())
                    if key not in data:
                        data[key] = []
                    data[key].append(value)
            print(f'### Results `{compiler} -O{optimization}` ({runs} runs * 10_000 iters)\n```')
            label_len = max([len(k) for k in data.keys()]) + 10
            cmp_data: dict[str, dict[str, dict[str, tuple[float, float]]]] = {}
            impl_label_len: int = 1
            for key, vs in data.items():
                vs_stats = stats(vs)
                stats_print(vs_stats, False, f'{key}: '.ljust(label_len))
                key_parts = key.split(':')
                impl_name = key_parts[0]
                if impl_name != 'str0':
                    impl_label_len = max(impl_label_len, len(impl_name))
                    test_i = key_parts[-1]
                    nl = data[f'str0:nl:{test_i}']
                    l = data[f'str0:l:{test_i}']
                    nl_pc = [(it-nl[i])/nl[i]*100 for i, it in enumerate(vs)]
                    l_pc = [(it-l[i])/l[i]*100 for i, it in enumerate(vs)]
                    nl_pc_stats = stats(nl_pc)
                    l_pc_stats = stats(l_pc)
                    stats_print(nl_pc_stats, True, f'  {key} dt/nl: '.ljust(label_len))
                    stats_print(l_pc_stats, True,  f'  {key}  dt/l: '.ljust(label_len))
                    for metric_name in ['avg', 'med', 'p5', 'p10', 'p90', 'p95']:
                        cmp_name = ':'.join(key_parts[1:])
                        cmp_values = nl_pc_stats[metric_name], l_pc_stats[metric_name]
                        if cmp_name not in cmp_data.keys():
                            cmp_data[cmp_name] = {}
                        if metric_name not in cmp_data[cmp_name].keys():
                            cmp_data[cmp_name][metric_name] = {}
                        cmp_data[cmp_name][metric_name][impl_name] = cmp_values
            print(f'```\n**Winners (draw/- is <{draw_treshold_pc}%):**\n```')
            for i in [0, 1]:
                print(f'{"nl" if i == 0 else "l"}')
                for cmp_name, metrics_data in cmp_data.items():
                    print(f'  {cmp_name}')
                    metric_label_len = max([len(k) for k in metrics_data.keys()]) + 2
                    for metric_name, impls_data in metrics_data.items():
                        best_impl_name: str|None = None
                        best_impl_res: float|None = None
                        worst_impl_res = max([t[i] for t in impls_data.values()])
                        best_impl_res_2: float = worst_impl_res
                        for impl_name, res_tuple in impls_data.items():
                            if best_impl_name is None or best_impl_res is None or res_tuple[i] < best_impl_res:
                                if best_impl_res is not None:
                                    best_impl_res_2 = best_impl_res
                                best_impl_res = res_tuple[i]
                                best_impl_name = impl_name
                        if best_impl_name is None or best_impl_res is None or best_impl_res_2 is None:
                            raise Exception('programming error, None found')
                        is_draw = worst_impl_res-best_impl_res < draw_treshold_pc
                        print('    {metric_name}{winner} ({best_impl_res:+7.2f}% | W: {winner_real} | W-W2: {gap:+7.2f}%)'.format(
                            metric_name=(metric_name + ': ').ljust(metric_label_len), 
                            winner=('-' if is_draw else best_impl_name).ljust(impl_label_len+1),
                            best_impl_res=best_impl_res,
                            winner_real=best_impl_name.ljust(impl_label_len),
                            gap=best_impl_res-best_impl_res_2,
                        ))
            print('```')

if __name__ == '__main__':
    main()

