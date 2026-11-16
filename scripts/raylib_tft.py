"""Gebruik TFT_eSPI voor het scherm en de ESP32-port voor raylib zelf."""

Import("env")

from pathlib import Path


def select_raylib_sources(env, node):
    path = node.srcnode().get_abspath().replace("\\", "/")
    marker = "/raylib-esp32/"
    if marker not in path:
        return node

    relative = path.split(marker, 1)[1]
    if relative == "raylib/src/rcore.c":
        # De vastgezette port zet Window.ready niet en start de timer niet.
        # Corrigeer een buildkopie; laat de gedownloade bron intact.
        source = Path(path).read_text(encoding="utf-8")
        old = "    CORE.Window.shouldClose = false;\n\n    // Initialize random seed"
        new = ("    CORE.Window.shouldClose = false;\n"
               "    CORE.Window.ready = true;\n"
               "    InitTimer();\n\n    // Initialize random seed")
        if source.count(old) != 1:
            raise RuntimeError("Onverwachte raylib-versie: initialisatiepatch past niet.")
        corrected = source.replace(old, new, 1)
        target = Path(env.subst("$BUILD_DIR")) / "pws_raylib" / "rcore_pws.c"
        target.parent.mkdir(parents=True, exist_ok=True)
        if not target.exists() or target.read_text(encoding="utf-8") != corrected:
            target.write_text(corrected, encoding="utf-8")
        return env.File(str(target))
    # Onze RaylibTftPort.cpp vervangt de SPI-driver van deze port.
    if relative.startswith(("src/drivers/", "src/port/", "src/fs/")):
        return None
    if relative == "src/raylib_esp32.c":
        return None
    return node


env.AddBuildMiddleware(select_raylib_sources)
