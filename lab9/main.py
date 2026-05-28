import itertools
import sys
import time
import multiprocessing
from PIL import Image


def process_fragment(pixel_chunk):
 #   res = []
 #   for (r,g,b) in pixel_chunk:
 #        res.append((0xff - r,0xff - g, 0xff - b))
 #   return res
    return [(0xff - r, 0xff - g, 0xff - b) for (r, g, b) in pixel_chunk]


def main():
    img_path = sys.argv[1]
    num_processes = int(sys.argv[2])
    img = Image.open(img_path).convert("RGB")

    width, height = img.size

    pixels = list(img.get_flattened_data())
    total = len(pixels)

    sizeT = total // num_processes

    chunks = []
    for i in range(num_processes):
        chunks.append(pixels[i * sizeT:(i + 1) * sizeT])

    start_time = time.time()
    print(f" width: {width}, height: {height}, total: {total}, sizeT: {sizeT}")

    with multiprocessing.Pool(processes=num_processes) as pool:
        processed_chunks = pool.map(process_fragment, chunks)

    final_pixels = list(itertools.chain.from_iterable(processed_chunks))

    print(time.time() - start_time)
    result_img = Image.new("RGB", (width, height))
    result_img.putdata(final_pixels)

    output_filename = img_path + "Negative.jpg"
    result_img.save(output_filename)


if __name__ == "__main__":
    main()