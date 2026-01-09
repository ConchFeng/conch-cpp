#compdef run.sh

# Zsh completion script for run.sh

_run_sh() {
    local -a executables
    
    # Check if build/bin directory exists
    if [[ ! -d "build/bin" ]]; then
        return 0
    fi
    
    # Collect available executables
    for exe in build/bin/*; do
        if [[ -f "$exe" && -x "$exe" ]]; then
            # Regular executable
            executables+=($(basename "$exe"))
        elif [[ -d "$exe" && "$exe" == *.app ]]; then
            # macOS .app bundle
            executables+=($(basename "$exe" .app))
        fi
    done
    
    # Provide completions
    _describe 'executable' executables
}

_run_sh "$@"
