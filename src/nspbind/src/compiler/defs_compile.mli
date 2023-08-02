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

(* $Id: defs_compile.mli,v 1.3 2019-05-21 12:00:43 jpc Exp $ *)

val compile : Path.file_name -> unit;;
(* From simport compatible input file to (target language) compiled file. *)

(*
 Local Variables:
  compile-command: "cd ../..; make"
  End:
*)
