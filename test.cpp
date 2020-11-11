#include "runtime.h"

double cue_DCH_419(double UE_k5, double QP_i1, double HH_x0, double RU_u1) {
    double _t2 = HH_x0 < QP_i1;
    if (_t2) {
        return QP_i1;
    } else {
        double _t6 = std::fmod(HH_x0, RU_u1);
        double LJ_s0 = _t6;
        return HH_x0;
    }
    double _t8 = 20914;
    double _t10 = _t8 <= QP_i1;
    if (_t10) {
        double _t12 = 546537;
        double _t13 = 189118;
        double _t14 = 349929;
        double _t15 = 159642;
        double _t16 = 962921;
        double _t17 = 909681;
        double _t18 = 171074;
        double _t19 = 658296;
        double _t20 = 63086;
        std::vector<double> _t11 = {_t12, _t13, _t14, _t15, _t16, _t17, _t18, _t19, _t20};
        std::vector<double> HH_k6 = _t11;
        return HH_x0;
    } else {
        double _t24 = std::pow(QP_i1, QP_i1);
        double VO_f5 = _t24;
        return HH_x0;
    }
    return UE_k5;
}

double aqh_VDB_003(std::vector<double> IB_m4, std::vector<double> NH_t3, std::vector<double> MH_l1, double OL_o6) {
    double _t0 = 395760;
    double VO_d4 = _t0;
    double _t2 = 307817;
    double _t3 = 911929;
    double _t4 = 191680;
    double _t5 = 251057;
    double _t6 = 53400;
    std::vector<double> _t1 = {_t2, _t3, _t4, _t5, _t6};
    double _t8 = 1;
    double _t10 = 307817;
    double _t11 = 911929;
    double _t12 = 191680;
    double _t13 = 251057;
    double _t14 = 53400;
    std::vector<double> _t9 = {_t10, _t11, _t12, _t13, _t14};
    double _t7 = _t9[_t8];
    double _t17 = 998544;
    double _t18 = 681390;
    double _t19 = 262718;
    double _t20 = 451151;
    double _t21 = 433169;
    double _t22 = 549406;
    std::vector<double> _t16 = {_t17, _t18, _t19, _t20, _t21, _t22};
    double _t15 = vector_size(_t16);
    double _t23 = _t7 > _t15;
    if (_t23) {
        double _t24 = 0;
        double _t27 = 4;
        std::vector<double> _t26 = slice(NH_t3, _t24, _t27);
        double _t28 = 0;
        double _t31 = 2;
        std::vector<double> _t30 = slice(MH_l1, _t28, _t31);
        std::vector<double> _t32 = concatenate(_t26, _t30);
        std::vector<double> YJ_t4 = _t32;
        double _t33 = 916814;
        return _t33;
    } else {
        double _t34 = 356507;
        return _t34;
    }
    double _t35 = 727853;
    double RT_s8 = _t35;
    double _t38 = 218694;
    double _t39 = 852178;
    double _t40 = 666732;
    double _t41 = 271744;
    double _t42 = 387949;
    double _t43 = 330561;
    double _t44 = 570072;
    double _t45 = 74304;
    std::vector<double> _t37 = {_t38, _t39, _t40, _t41, _t42, _t43, _t44, _t45};
    double _t36 = vector_size(_t37);
    double _t46 = 750807;
    double _t47 = _t36 > _t46;
    if (_t47) {
        double _t48 = 706028;
        return _t48;
    }
    double _t50 = 786133;
    double _t51 = 674429;
    double _t52 = 722270;
    double _t53 = 533401;
    double _t54 = 902365;
    std::vector<double> _t49 = {_t50, _t51, _t52, _t53, _t54};
    double _t56 = 215334;
    double _t57 = 288019;
    double _t58 = 29518;
    double _t59 = 524161;
    double _t60 = 404413;
    double _t61 = 249790;
    double _t62 = 34273;
    double _t63 = 565006;
    double _t64 = 668542;
    std::vector<double> _t55 = {_t56, _t57, _t58, _t59, _t60, _t61, _t62, _t63, _t64};
    std::vector<double> _t65 = concatenate(_t49, _t55);
    std::vector<double> JS_p4 = _t65;
    double _t66 = 833488;
    return _t66;
}

double psc_KHQ_878(double JL_u8, std::vector<double> AI_w3, std::vector<double> WR_l2, std::vector<double> SL_c8) {
    double _t0 = 966133;
    double _t1 = 652766;
    double _t2 = _t0 == _t1;
    double _t4 = 686518;
    double _t5 = 295131;
    double _t6 = 967751;
    double _t7 = 885312;
    double _t8 = 700992;
    double _t9 = 587110;
    std::vector<double> _t3 = {_t4, _t5, _t6, _t7, _t8, _t9};
    double _t11 = 5;
    double _t13 = 686518;
    double _t14 = 295131;
    double _t15 = 967751;
    double _t16 = 885312;
    double _t17 = 700992;
    double _t18 = 587110;
    std::vector<double> _t12 = {_t13, _t14, _t15, _t16, _t17, _t18};
    double _t10 = _t12[_t11];
    double _t19 = 86608;
    double _t20 = _t10 >= _t19;
    double _t21 = _t2 || _t20;
    if (_t21) {
        double _t22 = 904725;
        double UW_i4 = _t22;
        double _t23 = 203695;
        return _t23;
    }
    double _t26 = 273857;
    double _t27 = 780981;
    double _t28 = 371608;
    double _t29 = 940407;
    double _t30 = 908157;
    double _t31 = 671599;
    double _t32 = 206351;
    std::vector<double> _t25 = {_t26, _t27, _t28, _t29, _t30, _t31, _t32};
    double _t24 = vector_size(_t25);
    double _t33 = 521141;
    double _t34 = _t24 != _t33;
    double _t35 = 108653;
    double _t36 = 100664;
    double _t37 = _t35 != _t36;
    double _t38 = _t34 || _t37;
    if (_t38) {
        double _t39 = 0;
        double _t42 = 3;
        std::vector<double> _t41 = slice(SL_c8, _t39, _t42);
        std::vector<double> LM_g0 = _t41;
        double _t43 = 915858;
        return _t43;
    } else {
        double _t44 = 0;
        double _t47 = 3;
        std::vector<double> _t46 = slice(AI_w3, _t44, _t47);
        double _t48 = 0;
        double _t51 = 5;
        std::vector<double> _t50 = slice(AI_w3, _t48, _t51);
        std::vector<double> _t52 = concatenate(_t46, _t50);
        std::vector<double> HF_e4 = _t52;
        double _t53 = 853401;
        return _t53;
    }
    double _t54 = 742316;
    double PN_n1 = _t54;
    double _t55 = 826301;
    return _t55;
}

std::vector<double> rrw_JOC_430(std::vector<double> MG_w9, std::vector<double> DJ_o8, double JQ_n9, double EX_n4) {
    double _t0 = 374648;
    double _t1 = 740691;
    double _t2 = _t0 > _t1;
    double _t5 = 283596;
    double _t6 = 711268;
    double _t7 = 624401;
    double _t8 = 138094;
    double _t9 = 833894;
    double _t10 = 222101;
    double _t11 = 303493;
    double _t12 = 702160;
    std::vector<double> _t4 = {_t5, _t6, _t7, _t8, _t9, _t10, _t11, _t12};
    double _t3 = vector_size(_t4);
    double _t15 = 402540;
    double _t16 = 884970;
    double _t17 = 332725;
    double _t18 = 382830;
    double _t19 = 490214;
    std::vector<double> _t14 = {_t15, _t16, _t17, _t18, _t19};
    double _t13 = vector_size(_t14);
    double _t20 = _t3 >= _t13;
    double _t21 = _t2 || _t20;
    if (_t21) {
        double _t22 = 301092;
        double WO_v8 = _t22;
        double _t25 = cue_DCH_419(WO_v8, EX_n4, WO_v8, JQ_n9);
        double _t30 = JQ_n9 + _t25;
        double NT_x1 = _t30;
        double _t32 = 954646;
        double _t33 = 489818;
        double _t34 = 90744;
        double _t35 = 683388;
        double _t36 = 95643;
        std::vector<double> _t31 = {_t32, _t33, _t34, _t35, _t36};
        std::vector<double> YP_a5 = _t31;
        double _t37 = 0;
        double _t40 = 2;
        std::vector<double> _t39 = slice(YP_a5, _t37, _t40);
        std::vector<double> HE_f0 = _t39;
        return MG_w9;
    } else {
        double _t42 = 975346;
        double CQ_p2 = _t42;
        double _t44 = 880023;
        double _t45 = 399592;
        double _t46 = 998000;
        double _t47 = 384574;
        double _t48 = 105691;
        std::vector<double> _t43 = {_t44, _t45, _t46, _t47, _t48};
        return _t43;
    }
    double _t49 = 763764;
    double AM_f8 = _t49;
    double _t51 = 543482;
    double _t52 = 230136;
    double _t53 = 214348;
    double _t54 = 349137;
    double _t55 = 277696;
    double _t56 = 673555;
    double _t57 = 863845;
    std::vector<double> _t50 = {_t51, _t52, _t53, _t54, _t55, _t56, _t57};
    return _t50;
}

std::vector<double> xqt_INQ_093(std::vector<double> YX_d4, double AH_n6, std::vector<double> OY_q1, std::vector<double> GR_i6, std::vector<double> LU_u3) {
    return OY_q1;
}

double tbj_KHC_727(double JM_p4, double KM_f9) {
    double _t0 = 158914;
    return _t0;
}

std::vector<double> cxl_YIB_204(std::vector<double> OI_r7, double ML_u5) {
    double _t0 = 692941;
    double VK_f9 = _t0;
    double _t1 = 961692;
    double VC_e8 = _t1;
    double _t5 = 258754;
    double _t6 = 353636;
    double _t7 = 473684;
    double _t8 = 475403;
    double _t9 = 842814;
    double _t10 = 561867;
    std::vector<double> _t4 = {_t5, _t6, _t7, _t8, _t9, _t10};
    double _t12 = 121058;
    double _t13 = 371061;
    double _t14 = 486705;
    double _t15 = 658505;
    double _t16 = 899979;
    double _t17 = 302831;
    double _t18 = 447778;
    double _t19 = 483777;
    std::vector<double> _t11 = {_t12, _t13, _t14, _t15, _t16, _t17, _t18, _t19};
    double _t20 = 700514;
    double _t21 = 839144;
    std::vector<double> _t3 = rrw_JOC_430(_t4, _t11, _t20, _t21);
    std::vector<double> WH_g4 = _t3;
    double _t23 = 793058;
    double _t24 = 199676;
    double _t25 = 703600;
    double _t26 = 616683;
    double _t27 = 808270;
    std::vector<double> _t22 = {_t23, _t24, _t25, _t26, _t27};
    double _t29 = 4;
    double _t31 = 793058;
    double _t32 = 199676;
    double _t33 = 703600;
    double _t34 = 616683;
    double _t35 = 808270;
    std::vector<double> _t30 = {_t31, _t32, _t33, _t34, _t35};
    double _t28 = _t30[_t29];
    double _t38 = 220156;
    double _t39 = 729557;
    double _t40 = 479190;
    double _t41 = 738979;
    double _t42 = 592354;
    double _t43 = 88157;
    std::vector<double> _t37 = {_t38, _t39, _t40, _t41, _t42, _t43};
    double _t36 = vector_size(_t37);
    double _t44 = _t28 > _t36;
    if (_t44) {
        double _t46 = 765466;
        double _t47 = 802642;
        double _t48 = 485471;
        double _t49 = 486511;
        double _t50 = 470666;
        double _t51 = 875080;
        double _t52 = 556453;
        std::vector<double> _t45 = {_t46, _t47, _t48, _t49, _t50, _t51, _t52};
        return _t45;
    }
    double _t54 = cue_DCH_419(VC_e8, ML_u5, VC_e8, VC_e8);
    double _t60 = cue_DCH_419(VC_e8, VC_e8, VC_e8, VC_e8);
    double _t65 = std::pow(_t54, _t60);
    double IQ_u2 = _t65;
    double _t69 = 949352;
    double _t70 = 185483;
    double _t71 = 101981;
    double _t72 = 649849;
    double _t73 = 815370;
    double _t74 = 973000;
    double _t75 = 973512;
    double _t76 = 421404;
    std::vector<double> _t68 = {_t69, _t70, _t71, _t72, _t73, _t74, _t75, _t76};
    double _t78 = 730934;
    double _t79 = 954500;
    double _t80 = 492882;
    double _t81 = 847546;
    double _t82 = 138684;
    double _t83 = 264517;
    double _t84 = 568459;
    std::vector<double> _t77 = {_t78, _t79, _t80, _t81, _t82, _t83, _t84};
    double _t86 = 441437;
    double _t87 = 572170;
    double _t88 = 941408;
    double _t89 = 101556;
    double _t90 = 26510;
    std::vector<double> _t85 = {_t86, _t87, _t88, _t89, _t90};
    double _t67 = aqh_VDB_003(_t68, _t77, _t85, IQ_u2);
    double _t93 = std::fmod(_t67, IQ_u2);
    double CQ_w9 = _t93;
    double _t95 = 213681;
    double _t96 = 299016;
    double _t97 = 866154;
    double _t98 = 930944;
    double _t99 = 789000;
    double _t100 = 694801;
    double _t101 = 272562;
    double _t102 = 36915;
    std::vector<double> _t94 = {_t95, _t96, _t97, _t98, _t99, _t100, _t101, _t102};
    return _t94;
}

double rut_IOV_708(std::vector<double> YM_r6, double VR_q3, double JN_v1) {
    double _t0 = 132570;
    return _t0;
}

double uir_RBR_743(double JX_f3, std::vector<double> RH_v0) {
    double _t1 = 731009;
    double _t2 = 370786;
    double _t3 = 197192;
    double _t4 = 762638;
    double _t5 = 991671;
    double _t6 = 22111;
    double _t7 = 574809;
    double _t8 = 82679;
    std::vector<double> _t0 = {_t1, _t2, _t3, _t4, _t5, _t6, _t7, _t8};
    std::vector<double> DD_r9 = _t0;
    double _t10 = 722794;
    double _t11 = 147870;
    double _t12 = 310888;
    double _t13 = 266780;
    double _t14 = 502034;
    double _t15 = 321720;
    std::vector<double> _t9 = {_t10, _t11, _t12, _t13, _t14, _t15};
    double _t17 = 3;
    double _t19 = 722794;
    double _t20 = 147870;
    double _t21 = 310888;
    double _t22 = 266780;
    double _t23 = 502034;
    double _t24 = 321720;
    std::vector<double> _t18 = {_t19, _t20, _t21, _t22, _t23, _t24};
    double _t16 = _t18[_t17];
    double _t25 = 28703;
    double _t26 = _t16 > _t25;
    double _t27 = 976576;
    double _t30 = 788906;
    double _t31 = 701355;
    double _t32 = 642041;
    double _t33 = 303334;
    double _t34 = 695576;
    double _t35 = 661278;
    std::vector<double> _t29 = {_t30, _t31, _t32, _t33, _t34, _t35};
    double _t28 = vector_size(_t29);
    double _t36 = _t27 >= _t28;
    double _t37 = _t26 && _t36;
    if (_t37) {
        double _t38 = 434773;
        return _t38;
    } else {
        double _t39 = 162685;
        double HI_j4 = _t39;
        double _t41 = 114297;
        double _t42 = 842217;
        double _t43 = 642223;
        double _t44 = 514893;
        double _t45 = 401497;
        std::vector<double> _t40 = {_t41, _t42, _t43, _t44, _t45};
        std::vector<double> IL_h7 = _t40;
        double _t46 = 559456;
        return _t46;
    }
    double _t48 = 644756;
    double _t49 = 865213;
    double _t50 = 190394;
    double _t51 = 20098;
    double _t52 = 625290;
    double _t53 = 97961;
    double _t54 = 175800;
    double _t55 = 971101;
    double _t56 = 737389;
    std::vector<double> _t47 = {_t48, _t49, _t50, _t51, _t52, _t53, _t54, _t55, _t56};
    std::vector<double> OM_h7 = _t47;
    double _t57 = 454561;
    return _t57;
}

std::vector<double> rpj_XLO_087(double AA_q2, double HS_i0, double XN_n1) {
    double _t0 = 176890;
    double _t1 = 53272;
    double _t2 = std::floor(_t0 / _t1);
    double BS_p5 = _t2;
    double _t4 = 990608;
    double _t5 = 241670;
    double _t6 = 682239;
    double _t7 = 797872;
    double _t8 = 596013;
    std::vector<double> _t3 = {_t4, _t5, _t6, _t7, _t8};
    std::vector<double> DJ_o6 = _t3;
    double _t10 = 943320;
    double _t11 = 596959;
    double _t12 = 866145;
    double _t13 = 355955;
    double _t14 = 252906;
    double _t15 = 601018;
    std::vector<double> _t9 = {_t10, _t11, _t12, _t13, _t14, _t15};
    return _t9;
}

int main(int argc, char *argv[]) {
    std::vector<double> JX_n2 = obten_arreglo(argv[1]);
    double YL_i9 = obten_numero(argv[2]);
    double _t16 = 871909;
    double _t18 = 384683;
    double _t19 = 878639;
    double _t20 = 412288;
    double _t21 = 524950;
    double _t22 = 776824;
    std::vector<double> _t17 = {_t18, _t19, _t20, _t21, _t22};
    double _t24 = 1;
    double _t26 = 384683;
    double _t27 = 878639;
    double _t28 = 412288;
    double _t29 = 524950;
    double _t30 = 776824;
    std::vector<double> _t25 = {_t26, _t27, _t28, _t29, _t30};
    double _t23 = _t25[_t24];
    double _t31 = _t16 >= _t23;
    double _t34 = 166565;
    double _t35 = 779397;
    double _t36 = 799541;
    double _t37 = 810675;
    double _t38 = 568923;
    double _t39 = 823603;
    double _t40 = 99151;
    double _t41 = 548492;
    double _t42 = 307350;
    std::vector<double> _t33 = {_t34, _t35, _t36, _t37, _t38, _t39, _t40, _t41, _t42};
    double _t32 = vector_size(_t33);
    double _t43 = 203035;
    double _t44 = _t32 > _t43;
    double _t45 = _t31 || _t44;
    if (_t45) {
        double _t46 = 23900;
        std::cout << _t46 << '\n';
        return 0;
        return _t46;
    } else {
        double _t47 = 917989;
        std::cout << _t47 << '\n';
        return 0;
        return _t47;
    }
    double _t49 = 802214;
    double _t50 = 425332;
    double _t51 = 866731;
    double _t52 = 402300;
    double _t53 = 731941;
    double _t54 = 240535;
    double _t55 = 435580;
    double _t56 = 80313;
    double _t57 = 217468;
    std::vector<double> _t48 = {_t49, _t50, _t51, _t52, _t53, _t54, _t55, _t56, _t57};
    std::vector<double> OD_v4 = _t48;
    double _t58 = 999114;
    std::cout << _t58 << '\n';
    return 0;
    return _t58;
}
