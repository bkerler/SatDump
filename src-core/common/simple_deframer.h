#pragma once

#include <cstdint>
#include <vector>
#include <cmath>

namespace def
{
    class SimpleDeframer
    {
    private:
        const uint64_t d_syncword;
        const int d_syncword_length;
        const long d_sync_modulo;
        const int d_frame_length;
        const int d_thresold;
        const bool d_byte_aligned;

        bool in_frame = false;
        std::vector<uint8_t> current_frame;

        uint64_t asm_shifter = 0;

        int corr_64(uint64_t v1, uint64_t v2)
        {
            int cor = 0;
            uint64_t diff = v1 ^ v2;
            for (; diff; cor++)
                diff &= diff - 1;
            return cor;
        }

    public:
        SimpleDeframer(uint64_t syncword, int syncword_bit_length, int frame_length_bits, int thresold, bool byte_aligned = false)
            : d_syncword(syncword),
              d_syncword_length(syncword_bit_length),
              d_sync_modulo(pow(2, syncword_bit_length)),
              d_frame_length(frame_length_bits),
              d_thresold(thresold),
              d_byte_aligned(byte_aligned)
        {
        }

        std::vector<std::vector<uint8_t>> work(uint8_t *data, int size)
        {
            std::vector<std::vector<uint8_t>> output_frames;

            if (d_byte_aligned)
            {
                for (int byten = 0; byten < size; byten++)
                {
                    asm_shifter = (asm_shifter << 8 | data[byten]) % d_sync_modulo;

                    if (in_frame)
                    {
                        if (current_frame.size() == 0) // Push ASM
                        {
                            for (int shift = d_syncword_length - 8; shift >= 0; shift -= 8)
                            {
                                uint8_t byte = (d_syncword >> shift) & 0xFF;
                                current_frame.push_back(byte);
                            }
                        }

                        current_frame.push_back(data[byten]);

                        if (current_frame.size() * 8 == d_frame_length)
                        {
                            output_frames.push_back(current_frame);
                            in_frame = false;
                        }
                    }

                    if (corr_64(d_syncword, asm_shifter) < d_thresold)
                    {
                        if (in_frame)
                        {
                            // Fill up what we're missing
                            while (current_frame.size() * 8 < d_frame_length)
                                current_frame.push_back(0x00);
                            output_frames.push_back(current_frame);
                        }

                        in_frame = true;
                        current_frame.clear();
                    }
                }
            }
            else
            {
                // TODO
            }

            return output_frames;
        }
    };
};