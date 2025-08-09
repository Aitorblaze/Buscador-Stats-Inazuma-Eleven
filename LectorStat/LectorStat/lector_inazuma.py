def read16(data, pos):
    return data[pos] | (data[pos + 1] << 8)

def read8(data, pos):
    return data[pos]

def main():
    import sys

    if len(sys.argv) != 2:
        print("Uso: python lector_inazuma.py \"NombreJugador\"")
        sys.exit(1)

    target_name = sys.argv[1]
    pattern = target_name.encode('utf-8')

    try:
        with open("unitbase.dat", "rb") as f:
            data1 = f.read()
    except FileNotFoundError:
        print("No se pudo abrir unitbase.dat")
        sys.exit(1)

    try:
        with open("unitstat.dat", "rb") as f:
            data2 = f.read()
    except FileNotFoundError:
        print("No se pudo abrir unitstat.dat")
        sys.exit(1)

    pos_in_base = 0
    any_found = False
    count = 0

    while pos_in_base <= len(data1) - len(pattern):
        offset_base = data1.find(pattern, pos_in_base)
        if offset_base == -1:
            break

        unit_index = offset_base // 0x68
        offset_stats = unit_index * 0x48

        if offset_stats + 0x48 > len(data2):
            print("Aviso: Offset fuera de rango, se ignora.")
            pos_in_base = offset_base + 1
            continue

        print(f"=== Coincidencia #{count+1} en offset unitbase.dat 0x{offset_base:X} ===")

        pos = offset_stats
        freedom = 0

        fp_min = read16(data2, pos); pos+=2
        fp_max = read16(data2, pos); pos+=2
        freedom += read16(data2, pos) + read16(data2, pos+2)
        pos+=4

        tp_min = read16(data2, pos); pos+=2
        tp_max = read16(data2, pos); pos+=2
        freedom += read16(data2, pos) + read16(data2, pos+2)
        pos+=4

        kick_min = read8(data2, pos); pos+=1
        kick_max = read8(data2, pos); pos+=1
        freedom += read8(data2, pos) + read8(data2, pos+1)
        pos+=2

        body_min = read8(data2, pos); pos+=1
        body_max = read8(data2, pos); pos+=1
        freedom += read8(data2, pos) + read8(data2, pos+1)
        pos+=2

        guard_min = read8(data2, pos); pos+=1
        guard_max = read8(data2, pos); pos+=1
        freedom += read8(data2, pos) + read8(data2, pos+1)
        pos+=2

        control_min = read8(data2, pos); pos+=1
        control_max = read8(data2, pos); pos+=1
        freedom += read8(data2, pos) + read8(data2, pos+1)
        pos+=2

        speed_min = read8(data2, pos); pos+=1
        speed_max = read8(data2, pos); pos+=1
        freedom += read8(data2, pos) + read8(data2, pos+1)
        pos+=2

        guts_min = read8(data2, pos); pos+=1
        guts_max = read8(data2, pos); pos+=1
        freedom += read8(data2, pos) + read8(data2, pos+1)
        pos+=2

        stamina_min = read8(data2, pos); pos+=1
        stamina_max = read8(data2, pos); pos+=1
        freedom += read8(data2, pos) + read8(data2, pos+1)
        pos+=2

        total = kick_max + body_max + guard_max + control_max + speed_max + guts_max + stamina_max

        print(f"FP:       min: {fp_min}, max: {fp_max}")
        print(f"TP:       min: {tp_min}, max: {tp_max}")
        print(f"Kick:     min: {kick_min}, max: {kick_max}")
        print(f"Body:     min: {body_min}, max: {body_max}")
        print(f"Guard:    min: {guard_min}, max: {guard_max}")
        print(f"Control:  min: {control_min}, max: {control_max}")
        print(f"Speed:    min: {speed_min}, max: {speed_max}")
        print(f"Guts:     min: {guts_min}, max: {guts_max}")
        print(f"Stamina:  min: {stamina_min}, max: {stamina_max}")
        print(f"Freedom:  {freedom}")
        print(f"Total:    {total}\n")

        any_found = True
        count += 1
        pos_in_base = offset_base + 1

    if not any_found:
        print("Jugador no encontrado en unitbase.dat")

if __name__ == "__main__":
    main()