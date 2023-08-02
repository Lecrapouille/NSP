(***********************************************************************)
(*                                                                     *)
(*                               Interface generator                   *)
(*                                                                     *)
(*                   Pierre Weis, INRIA Rocquencourt                   *)
(*                                                                     *)
(*  Copyright 2010-2015,                                               *)
(*  Institut National de Recherche en Informatique et en Automatique.  *)
(*  All rights reserved.                                               *)
(*                                                                     *)
(*  This file is distributed under the terms of the BSD License.       *)
(*                                                                     *)
(***********************************************************************)

(* $Id: main_gen.mli,v 1.2 2015-02-13 10:17:37 jpc Exp $ *)

val do_phase : string -> ('a -> string -> unit) -> 'a -> string -> unit
;;

(*
 Local Variables:
  compile-command: "cd ../../..; make"
  End:
*)
