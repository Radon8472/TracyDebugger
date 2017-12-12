<?php

$p = $this->wire('pages')->get((int)$_POST['goToPage']);
$pageInfo = array();
if($p->id) {
	$pageInfo['id'] = $p->id;
	$pageInfo['title'] = truncateText($p->title, 50);
	$pageInfo['url'] = $p->url;
	$pageInfo['template'] = '<a href="'.$this->wire('config')->urls->admin.'setup/template/edit?id='.$p->template->id.'">'.$p->template->name.'</a>';
}
echo json_encode($pageInfo);
exit;

function truncateText($rawText, $maxlength) {
  // truncate to max length
  $text = substr(strip_tags($rawText), 0, $maxlength);
  // check if we've truncated to a spot that needs further truncation
  if(strlen(rtrim($text, ' .!?,;')) == $maxlength) {
    // truncate to last word
    $text = substr($text, 0, strrpos($text, ' '));
  }
  return trim($text) . (strlen($rawText) > $maxlength ? '&hellip;' : '');
}