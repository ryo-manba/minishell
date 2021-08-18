import * as MS from "./minishell";
import { set, del } from '@vue/composition-api';
export type ShellVarMap = {
    [key: string]: MS.ShellVariable;
};

export function load_vars() {
    const stored_str = localStorage.getItem("minishell_vars");
    const vars = (stored_str ? JSON.parse(stored_str) : {}) as ShellVarMap;
    console.log(vars);
    return vars;
}

export function store_vars(varmap: ShellVarMap) {
    console.log(varmap);
    localStorage.setItem("minishell_vars", JSON.stringify(varmap));
}

export function export_var(varmap: ShellVarMap, key: string) {
    if (!varmap[key]) {
        set(varmap, key, {
            key,
            value: null,
            is_env: true,
            attr: 0,
        });
    } else {
        set(varmap[key], `is_env`, true);
    }
}

export function unpublic_var(varmap: ShellVarMap, key: string) {
    if (!varmap[key]) { return }
    set(varmap[key], `is_env`, false);
}

export function unset_var(varmap: ShellVarMap, key: string) {
    del(varmap, key);
}

export function assign_var(varmap: ShellVarMap, key: string, value: string) {
    if (!varmap[key]) {
        set(varmap, key, {
            key,
            value: null,
            is_env: false,
            attr: 0,
        });
    }
    varmap[key].value = value;
}

export function get_var(varmap: ShellVarMap, key: string) {
    if (!varmap[key] || typeof varmap[key].value !== "string") { return null; }
    return varmap[key].value;
}
