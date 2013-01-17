<?php
class WebServiceException extends Exception {
	private $errorsList = array();
	
	public function getErrorsList() {
		return $this->errorsList;
	}
	
	public function setErrorsList($list) {
		$this->errorsList = $list;
	}
	
	public function addError($error) {
		array_push($this->errorsList, $error);
	}
	
	public function addErrors($errorsList) {
		foreach($errorsList as $error) {
			array_push($this->errorsList, $error);
		}
	}
}