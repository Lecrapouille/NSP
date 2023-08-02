(***********************************************************************)
(*                                                                     *)
(*                           Interface generator                       *)
(*                                                                     *)
(*          J.Ph Chancelier, Enpc/Cermics                              *)
(*                                                                     *)
(*  Copyright 2012-2015,                                               *)
(*  Ecole Nationale des ponts et chaussees                             *)
(*  All rights reserved.                                               *)
(*                                                                     *)
(*  This file is distributed under the terms of the BSD License.       *)
(*                                                                     *)
(***********************************************************************)

(* $Id: lisp_to_ast.mli,v 1.3 2015-02-13 10:17:38 jpc Exp $ *)

(** Entry points to the parser *)

val read_lisp_file :
    Path.explicit_file_name -> Lisp_ast.t list * Lisp_ast.t list ;;

(*
 Local Variables:
  compile-command: "cd ../..; make"
  End:
*)