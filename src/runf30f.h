    opcode = F8;
    switch(opcode) {

    case 0x10:  /* MOVSS Gx Ex */
        nextop = F8;
        GET_EX;
        GX.ud[0] = EX->ud[0];
        if((nextop&0xC0)!=0xC0) {
            // op2 is not a register
            GX.ud[1] = GX.ud[2] = GX.ud[3] = 0;
        }
        break;
    case 0x11:  /* MOVSS Ex Gx */
        nextop = F8;
        GET_EX;
        EX->ud[0] = GX.ud[0];
        break;

    case 0x2A:  /* CVTSI2SS Gx, Ed */
        nextop = F8;
        GET_ED;
        GX.f[0] = ED->sdword[0];
        break;

    case 0x2C:  /* CVTTSS2SI Gd, Ex */
    case 0x2D:  /* CVTSS2SI Gd, Ex */
        nextop = F8;
        GET_EX;
        GD.sdword[0] = EX->f[0];
        break;

    case 0x51:  /* SQRTSS Gx, Ex */
        nextop = F8;
        GET_EX;
        GX.f[0] = sqrtf(EX->f[0]);
        break;

    case 0x58:  /* ADDSS Gx, Ex */
        nextop = F8;
        GET_EX;
        GX.f[0] += EX->f[0];
        break;
    case 0x59:  /* MULSS Gx, Ex */
        nextop = F8;
        GET_EX;
        GX.f[0] *= EX->f[0];
        break;
    case 0x5A:  /* CVTSS2SD Gx, Ex */
        nextop = F8;
        GET_EX;
        GX.d[0] = EX->f[0];
        break;
    case 0x5B:  /* CVTTPS2DQ Gx, Ex */
        nextop = F8;
        GET_EX;
        GX.sd[0] = EX->f[0];
        GX.sd[1] = EX->f[1];
        GX.sd[2] = EX->f[2];
        GX.sd[3] = EX->f[3];
        break;

    case 0x5C:  /* SUBSS Gx, Ex */
        nextop = F8;
        GET_EX;
        GX.f[0] -= EX->f[0];
        break;
    case 0x5D:  /* MINSS Gx, Ex */
        nextop = F8;
        GET_EX;
        if(isnan(GX.f[0]) || isnan(EX->f[0]) || isless(EX->f[0], GX.f[0]))
            GX.f[0] = EX->f[0];
        break;
    case 0x5E:  /* DIVSS Gx, Ex */
        nextop = F8;
        GET_EX;
        GX.f[0] /= EX->f[0];
        break;
    case 0x5F:  /* MAXSS Gx, Ex */
        nextop = F8;
        GET_EX;
        if (isnan(GX.f[0]) || isnan(EX->f[0]) || isgreater(EX->f[0], GX.f[0]))
            GX.f[0] = EX->f[0];
        break;

    case 0x6F:  /* MOVDQU Gx, Ex */
        nextop = F8;
        GET_EX;
        memcpy(EX, &GD, 16);
        break;
    case 0x70:  /* PSHUFHW Gx, Ex, Ib */
        nextop = F8;
        GET_EX;
        tmp8u = F8;
        if(&GX==EX) {
            for (int i=0; i<4; ++i)
                eax1.uw[4+i] = EX->uw[4+((tmp8u>>(i*2))&3)];
            GX.q[1] = eax1.q[1];
        } else {
            for (int i=0; i<4; ++i)
                GX.uw[4+i] = EX->uw[4+((tmp8u>>(i*2))&3)];
            GX.q[0] = EX->q[0];
        }
        break;

    case 0x7E:  /* MOVQ Gx, Ex */
        nextop = F8;
        GET_EX;
        GX.q[0] = EX->q[0];
        GX.q[1] = 0;
        break;
    case 0x7F:  /* MOVDQU Ex, Gx */
        nextop = F8;
        GET_EX;
        EX->q[0] = GX.q[0];
        EX->q[1] = GX.q[1];
        break;

    case 0xC2:  /* CMPSS Gx, Ex, Ib */
        nextop = F8;
        GET_EX;
        tmp8u = F8;
        tmp8s = 0;
        switch(tmp8u&7) {
            case 0: tmp8s=(GX.f[0] == EX->f[0]); break;
            case 1: tmp8s=isless(GX.f[0], EX->f[0]); break;
            case 2: tmp8s=islessequal(GX.f[0], EX->f[0]); break;
            case 3: tmp8s=isnan(GX.f[0]) || isnan(EX->f[0]); break;
            case 4: tmp8s=(GX.f[0] != EX->f[0]); break;
            case 5: tmp8s=isnan(GX.f[0]) || isnan(EX->f[0]) || isgreaterequal(GX.f[0], EX->f[0]); break;
            case 6: tmp8s=isnan(GX.f[0]) || isnan(EX->f[0]) || isgreater(GX.f[0], EX->f[0]); break;
            case 7: tmp8s=!isnan(GX.f[0]) && !isnan(EX->f[0]); break;
        }
        if(tmp8s)
            GX.ud[0] = 0xffffffff;
        else
            GX.ud[0] = 0;
        break;

    case 0xD6:  /* MOVQ2DQ Gx, Em */
        nextop = F8;
        GET_EM;
        GX.q[0] = EM->q;
        GX.q[1] = 0;
        break;

    case 0xE6:  /* CVTDQ2PD Gx, Ex */
        nextop = F8;
        GET_EX;
        GX.d[1] = EX->sd[1];
        GX.d[0] = EX->sd[0];
        break;

    default:
        goto _default;
    }