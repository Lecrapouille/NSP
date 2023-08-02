(TeX-add-style-hook
 "data_desc"
 (lambda ()
   (TeX-run-style-hooks
    "insz"
    "inptr"
    "outsz"
    "outptr"
    "opar"
    "oz"
    "org_scicos"
    "org_scicos_func"
    "org_cdgen"
    "param_struc"
    "orgcallblk")
   (LaTeX-add-labels
    "trois"
    "deux"
    "guiii"
    "org_scicos"
    "org_scicos_func"
    "org_cdgen"
    "diagr_gui"
    "gui"
    "gui2"
    "gui3"
    "param_struc"
    "Flowchart"
    "guix"
    "throt2"
    "costx"
    "elect")
   (LaTeX-add-bibitems
    "Campbell05"
    "Chancelier2002"
    "sundials1"
    "sundials2"
    "spice"
    "hompackx")))

