add_rules("mode.debug", "mode.release")

target("base_64")
    set_languages("c++17")
    set_targetdir("$(projectdir)")
    set_kind("binary")
    add_files("src/*.cpp")
    add_links("ssl", "crypto")