    nextop = F8;
    switch (nextop) {
        case 0xC0:
        case 0xC1:
        case 0xC2:
        case 0xC3:
        case 0xC4:
        case 0xC5:
        case 0xC6:
        case 0xC7:  /* FLD STx */
            ll = ST(nextop&7).ll;
            fpu_do_push(emu);
            ST0.ll = ll;
            break;
        case 0xC8:
        case 0xC9:
        case 0xCA:
        case 0xCB:
        case 0xCC:
        case 0xCD:
        case 0xCE:
        case 0xCF:  /* FXCH STx */
            ll = ST(nextop&7).ll;
            ST(nextop&7).ll = ST0.ll;
            ST0.ll = ll;
            break;

        case 0xD0:  /* FNOP */
            break;

        case 0xE0:  /* FCHS */
            ST0.d = -ST0.d;
            break;
        case 0xE1:  /* FABS */
            ST0.d = fabs(ST0.d);
            break;
        
        case 0xE5:  /* FXAM */
            fpu_fxam(emu);
            break;

        case 0xE8:  /* FLD1 */
            fpu_do_push(emu);
            ST0.d = 1.0;
            break;
        case 0xE9:  /* FLDL2T */
            fpu_do_push(emu);
            ST0.d = L2T;
            break;
        case 0xEA:  /* FLDL2E */
            fpu_do_push(emu);
            ST0.d = L2E;
            break;
        case 0xEB:  /* FLDPI */
            fpu_do_push(emu);
            ST0.d = PI;
            break;
        case 0xEC:  /* FLDLG2 */
            fpu_do_push(emu);
            ST0.d = LG2;
            break;
        case 0xED:  /* FLDLN2 */
            fpu_do_push(emu);
            ST0.d = LN2;
            break;
        case 0xEE:  /* FLDZ */
            fpu_do_push(emu);
            ST0.d = 0.0;
            break;
        
        case 0xF2:  /* FTAN */
            ST0.d = tan(ST0.d);
            fpu_do_push(emu);
            ST0.d = 1.0;
            break;
        case 0xF3:  /* FPATAN */
            ST1.d = atan2(ST1.d, ST0.d);
            fpu_do_pop(emu);
            break;

        case 0xFA:  /* FSQRT */
            ST0.d = sqrt(ST0.d);
            break;
        case 0xFB:  /* FSINCOS */
            fpu_do_push(emu);
            sincos(ST1.d, &ST1.d, &ST0.d);
            break;
        case 0xFC:  /* FRNDINT */
            ST0.d = fpu_round(emu, ST0.d);
            break;

        case 0xFE:  /* FSIN */
            ST0.d = sin(ST0.d);
            break;
        case 0xFF:  /* FCOS */
            ST0.d = cos(ST0.d);
            break;


        case 0xE4:
        case 0xF0:
        case 0xF1:
        case 0xF4:
        case 0xF5:
        case 0xF6:
        case 0xF7:
        case 0xF8:
        case 0xF9:
        case 0xFD:
            goto _default;
        default:
        switch((nextop>>3)&7) {
            case 0:     /* FLD ST0, Ed float */
                GET_ED;
                fpu_do_push(emu);
                ST0.d = *(float*)ED;
                break;
            case 2:     /* FST Ed, ST0 */
                GET_ED;
                *(float*)ED = ST0.d;
                break;
            case 3:     /* FSTP Ed, ST0 */
                GET_ED;
                *(float*)ED = ST0.d;
                fpu_do_pop(emu);
                break;
            case 4:     /* FLDENV m */
                // warning, incomplete
                GET_ED;
                fpu_loadenv(emu, (char*)ED, 0);
                break;
            case 5:     /* FLDCW Ew */
                GET_EW;
                emu->cw = EW->word[0];
                // do something with cw?
                emu->round = (fpu_round_t)((emu->cw >> 10) & 3);
                break;
            case 6:     /* FNSTENV m */
                // warning, incomplete
                GET_ED;
                fpu_savenv(emu, (char*)ED, 0);
                ED->dword[0] = emu->cw;
                ED->dword[1] = emu->sw.x16;
                // tagword: 2bits*8
                // intruction pointer: 48bits
                // data (operand) pointer: 48bits
                // last opcode: 11bits save: 16bits restaured (1st and 2nd opcode only)
                break;
            case 7: /* FNSTCW Ew */
                GET_EW;
                EW->word[0] = emu->cw;
                break;
            default:
                goto _default;
        }
    }