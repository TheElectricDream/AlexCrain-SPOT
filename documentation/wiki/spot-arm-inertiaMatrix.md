# spot.arm.inertiaMatrix

**File:** `+spot/+arm/inertiaMatrix.m` · **Package:** [[spot.arm|spot-arm]]

<!-- source: +spot/+arm/inertiaMatrix.m -->

## What it is for

The 6×6 mass matrix $M(\mathbf{q})$ of the Chaser with its three-link arm, free floating, for $\mathbf{q} = [x\ y\ \theta\ q_1\ q_2\ q_3]$ ([[The Arm|Math-Arm]]). It was derived with the Symbolic Math Toolbox in 2017 from the kinetic energy of the four bodies and generated as code; it was the diagram's *Inertia Function 1* block and is here unchanged.

## How to call it

```matlab
M = spot.arm.inertiaMatrix(armModel, x);   % x = [x y theta q1 q2 q3 ...]; only x(3:6) are used
```

`armModel` needs `mRED`, `IRED`, `Phi`, `b0`, `a1`, `b1`, `a2`, `b2`, `a3`, `b3`, `mPROX`, `mDIST`, `mEE`, `IPROX`, `IDIST`, `IEE`.

## Reading the generated code

**Lines 9–10.** Unpack the parameters into the generated function.

**Lines 15–18.** The body attitude and the three joint angles.

**Lines 20–134.** Intermediate terms `t2 … t116`: sums of link mass-length products (e.g. `t8 = a1·mPROX + (a1+b1)·(mDIST+mEE)`, the first moment of everything beyond the shoulder about it), cosines and sines of cumulative angles (`t6 = θ + q1`, `t12 = θ + q1 + q2`, `t15 = θ + q1 + q2 + q3`), and the mount angle `π/2 − Φ`.

**Line 135.** The matrix, written column by column (`reshape` of 36 values). Its structure: the top-left 2×2 is the total mass; the (1:2, 3:6) blocks are the first moments of the bodies about the base centre and each joint (how a translation couples with rotations); the (3:6, 3:6) block is the inertias of the links about the base and the joints, with the `cos(q2)`, `cos(q3)`, `cos(q2+q3)` terms that depend on the arm's configuration. The matrix is symmetric and positive definite.

<!-- xref -->
- **Used by:** `+spot/+arm/Plant.m`, `+spot/+arm/coriolisMatrix.m`, `+spot/+arm/loadCalibration.m`
- **Uses:** [[spot.arm.defaults|spot-arm-defaults]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+arm/inertiaMatrix.m</b> (136 lines; click to show)</summary>

```matlab
   1  function M = inertiaMatrix(p, x)
   2  %INERTIAMATRIX Mass matrix of the Chaser (RED) with its three-link arm, free floating.
   3  %   M = spot.arm.inertiaMatrix(p, x) for the state x = [x y theta q1 q2 q3 ...] (only x(3:6) are
   4  %   used) and the parameters p (mRED, IRED, Phi, b0, a1, b1, a2, b2, a3, b3, mPROX, mDIST, mEE,
   5  %   IPROX, IDIST, IEE; see spot.arm.defaults). M is 6 x 6, for [x y theta q1 q2 q3].
   6  %   Generated with the Symbolic Math Toolbox (2017); moved here from the diagram's "Inertia
   7  %   Function 1" block, unchanged.
   8  %#codegen
   9  M = inertiaFunc3Link(p.mRED, p.IRED, p.Phi, p.b0, p.a1, p.b1, p.a2, p.b2, p.a3, p.b3, p.mPROX, p.mDIST, ...
  10                       p.mEE, p.IPROX, p.IDIST, x, p.IEE);
  11  end
  12  
  13  function InertiaS = inertiaFunc3Link(mRED, IRED, Phi, b0, a1, b1, a2, b2, a3, b3, mPROX, mDIST, mEE, IPROX, IDIST, x, IEE)
  14  
  15      q0 = x(3);
  16      q1 = x(4);
  17      q2 = x(5);
  18      q3 = x(6);
  19      
  20      t2 = a1+b1;
  21      t3 = a1.*mPROX;
  22      t4 = mDIST.*t2;
  23      t5 = mEE.*t2;
  24      t6 = q0+q1;
  25      t7 = cos(t6);
  26      t8 = t3+t4+t5;
  27      t9 = a2.*mDIST;
  28      t10 = a2+b2;
  29      t11 = mEE.*t10;
  30      t12 = q0+q1+q2;
  31      t13 = cos(t12);
  32      t14 = t9+t11;
  33      t15 = q0+q1+q2+q3;
  34      t16 = cos(t15);
  35      t17 = mRED+mPROX+mDIST+mEE;
  36      t18 = b0.*mPROX;
  37      t19 = b0.*mDIST;
  38      t20 = b0.*mEE;
  39      t21 = t18+t19+t20;
  40      t22 = (pi/2-Phi)+q0;
  41      t23 = sin(t6);
  42      t24 = sin(t12);
  43      t25 = sin(t15);
  44      t26 = a2.*mEE;
  45      t27 = b2.*mEE;
  46      t28 = t9+t26+t27;
  47      t29 = sin(t22);
  48      t86 = t7.*t8;
  49      t87 = t13.*t14;
  50      t88 = a3.*mEE.*t16;
  51      t30 = -t86-t87-t88-t21.*t29;
  52      t31 = cos(t22);
  53      t32 = t21.*t31;
  54      t89 = t8.*t23;
  55      t90 = t14.*t24;
  56      t91 = a3.*mEE.*t25;
  57      t33 = t32-t89-t90-t91;
  58      t34 = a1.^2;
  59      t35 = a2.^2;
  60      t36 = b0.^2;
  61      t37 = b1.^2;
  62      t38 = a1.*a2.*mDIST.*2.0;
  63      t39 = a1.*a2.*mEE.*2.0;
  64      t40 = a2.*b1.*mDIST.*2.0;
  65      t41 = a1.*b2.*mEE.*2.0;
  66      t42 = a2.*b1.*mEE.*2.0;
  67      t43 = b1.*b2.*mEE.*2.0;
  68      t44 = t38+t39+t40+t41+t42+t43;
  69      t45 = cos(q2);
  70      t46 = t44.*t45;
  71      t47 = a2.*a3.*mEE.*2.0;
  72      t48 = a3.*b2.*mEE.*2.0;
  73      t49 = t47+t48;
  74      t50 = cos(q3);
  75      t51 = t49.*t50;
  76      t52 = a1.*a3.*mEE.*2.0;
  77      t53 = a3.*b1.*mEE.*2.0;
  78      t54 = t52+t53;
  79      t55 = q2+q3;
  80      t56 = cos(t55);
  81      t57 = t54.*t56;
  82      t58 = (pi/2-Phi)-q1;
  83      t59 = sin(t58);
  84      t60 = -(pi/2-Phi)+q1+q2;
  85      t61 = sin(t60);
  86      t62 = -(pi/2-Phi)+q1+q2+q3;
  87      t63 = sin(t62);
  88      t64 = mPROX.*t34;
  89      t65 = mDIST.*t34;
  90      t66 = mEE.*t34;
  91      t67 = mDIST.*t35;
  92      t68 = mEE.*t35;
  93      t69 = a3.^2;
  94      t70 = mEE.*t69;
  95      t71 = mDIST.*t37;
  96      t72 = mEE.*t37;
  97      t73 = b2.^2;
  98      t74 = mEE.*t73;
  99      t75 = a1.*b1.*mDIST.*2.0;
 100      t76 = a1.*b1.*mEE.*2.0;
 101      t77 = a2.*b2.*mEE.*2.0;
 102      t78 = a2.*b0.*mDIST;
 103      t79 = a2.*b0.*mEE;
 104      t80 = b0.*b2.*mEE;
 105      t81 = t78+t79+t80;
 106      t82 = a1.*a3.*mEE;
 107      t83 = a3.*b1.*mEE;
 108      t84 = t82+t83;
 109      t85 = t56.*t84;
 110      t92 = a1.*b0.*mPROX;
 111      t93 = a1.*b0.*mDIST;
 112      t94 = a1.*b0.*mEE;
 113      t95 = b0.*b1.*mDIST;
 114      t96 = b0.*b1.*mEE;
 115      t97 = t92+t93+t94+t95+t96;
 116      t98 = t59.*t97;
 117      t112 = t61.*t81;
 118      t113 = a3.*b0.*mEE.*t63;
 119      t99 = IPROX+IDIST+IEE+t46+t51+t57+t64+t65+t66+t67+t68+t70+t71+t72+t74+t75+t76+t77+t98-t112-t113;
 120      t100 = a1.*a2.*mDIST;
 121      t101 = a1.*a2.*mEE;
 122      t102 = a2.*b1.*mDIST;
 123      t103 = a1.*b2.*mEE;
 124      t104 = a2.*b1.*mEE;
 125      t105 = b1.*b2.*mEE;
 126      t106 = t100+t101+t102+t103+t104+t105;
 127      t107 = t45.*t106;
 128      t108 = a2.*a3.*mEE;
 129      t109 = a3.*b2.*mEE;
 130      t110 = t108+t109;
 131      t111 = t50.*t110;
 132      t114 = IDIST+IEE+t51+t67+t68+t70+t74+t77+t85+t107;
 133      t115 = IEE+t70+t85+t111;
 134      t116 = IEE+t70+t111;
 135      InertiaS = reshape([t17,0.0,t30,-t86-t87-t88,-t88-t13.*t28,-t88,0.0,t17,t33,-t89-t90-t91,-t91-t24.*t28,-t91,t30,t33,IRED+IPROX+IDIST+IEE+t46+t51+t57+t64+t65+t66+t67+t68+t70+t71+t72+t74+t75+t76+t77+t59.*(a1.*b0.*mPROX.*2.0+a1.*b0.*mDIST.*2.0+a1.*b0.*mEE.*2.0+b0.*b1.*mDIST.*2.0+b0.*b1.*mEE.*2.0)+mPROX.*t36+mDIST.*t36+mEE.*t36-t61.*(a2.*b0.*mDIST.*2.0+a2.*b0.*mEE.*2.0+b0.*b2.*mEE.*2.0)-a3.*b0.*mEE.*t63.*2.0,t99,IDIST+IEE+t51+t67+t68+t70+t74+t77+t85+t107-t112-t113,IEE+t70+t85+t111-t113,-t7.*t8-t13.*t14-a3.*mEE.*t16,-t8.*t23-t14.*t24-a3.*mEE.*t25,t99,IPROX+IDIST+IEE+t46+t51+t57+t64+t65+t66+t67+t68+t70+t71+t72+t74+t75+t76+t77,t114,t115,-t13.*t28-a3.*mEE.*t16,-t24.*t28-a3.*mEE.*t25,IDIST+IEE+t51+t67+t68+t70+t74+t77+t85+t107-t61.*t81-a3.*b0.*mEE.*t63,t114,IDIST+IEE+t51+t67+t68+t70+t74+t77,t116,-a3.*mEE.*t16,-a3.*mEE.*t25,IEE+t70+t85+t111-a3.*b0.*mEE.*t63,t115,t116,IEE+t70],[6,6]);
 136  end
```

</details>
<!-- /listing -->
