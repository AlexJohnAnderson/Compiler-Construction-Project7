set breakpoint pending on
set confirm off
file ./dgc
break drewgon::Err::report
commands
	where
end
break drewgon::ToDoError::ToDoError
commands
	where
end
break drewgon::InternalError::InternalError
commands
	where
end

define t7
  set args p7_tests/$arg0.dgc -o --
  run
end
